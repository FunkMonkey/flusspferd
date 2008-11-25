// vim:ts=2:sw=2:expandtab:autoindent:filetype=cpp:
/*
Copyright (c) 2008 Aristid Breitkreuz, Ruediger Sonderfeld

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "flusspferd/xml/document.hpp"
#include "flusspferd/xml/node.hpp"
#include "flusspferd/local_root_scope.hpp"
#include "flusspferd/string.hpp"
#include "flusspferd/exception.hpp"
#include "flusspferd/tracer.hpp"

using namespace flusspferd;
using namespace flusspferd::xml;

document::document(xmlDocPtr ptr)
  : node(xmlNodePtr(ptr))
{
}

static xmlDocPtr new_doc(call_context &) {
  xmlDocPtr ptr = xmlNewDoc((xmlChar const *) "1.0");
  if (!ptr)
    throw exception("Could not create empty XML document");
  return ptr;
}

document::document(call_context &x)
  : node(xmlNodePtr(new_doc(x)))
{
}

document::~document() {
  if (c_obj()->_private == get_gcptr()) {
    xmlFreeDoc(c_obj());
    set_c_obj(0);
  }
}

void document::post_initialize() {
  node::post_initialize();

  register_native_method("dump", &document::dump);
  register_native_method("copy", &document::copy);

  define_native_property("rootElement", permanent_property, &document::prop_root_element);
}

object document::class_info::create_prototype() {
  local_root_scope scope;

  object proto = node::class_info::create_prototype();

  create_native_method(proto, "dump", 0);
  create_native_method(proto, "copy", 1);

  return proto;
}

char const *document::class_info::constructor_name() {
  return "Document";
}

std::size_t document::class_info::constructor_arity() {
  return 0;
}

string document::dump() {
  xmlChar *doc_txt;
  int doc_txt_len;

  xmlDocDumpFormatMemoryEnc(c_obj(), &doc_txt, &doc_txt_len, "UTF-16", 1);

  char16_t *txt = (char16_t *) doc_txt;
  int txt_len = doc_txt_len / sizeof(char16_t);

  if (txt_len > 0 && *txt == 0xFEFF) {
    ++txt;
    --txt_len;
  }

  string str(txt, txt_len);

  xmlFree(doc_txt);

  return str;
}

object document::copy(bool recursive) {
  xmlDocPtr copy = xmlCopyDoc(c_obj(), recursive);

  if (!copy)
    throw exception("Could not copy XML document");

  return create_native_object<document>(get_prototype(), copy);
}

void document::prop_root_element(property_mode mode, value &data) {
  xmlNodePtr node;

  switch (mode) {
  case property_set:
    if (data.is_null() || data.is_void()) {
      node = 0;
      data = object();
    } else if (data.is_object()) {
      object obj = data.get_object();
      xml::node *p = dynamic_cast<xml::node*>(native_object_base::get_native(obj));
      if (!p) {
        data = value();
        break;
      }
      node = p->c_obj();
    } else {
      data = value();
      break;
    }

    if (node && node->type != XML_ELEMENT_NODE) {
      data = object();
      break;
    }

    if (node) {
      xmlNodePtr old = xmlDocSetRootElement(c_obj(), node);
      if (old && !old->_private)
        xmlFreeNode(old);
    } else {
      xmlNodePtr old = xmlDocGetRootElement(c_obj());
      if (!old)
        break;
      xmlUnlinkNode(old);
      if (!old->_private)
        xmlFreeNode(old);
    }
    break;
  case property_get:
    node = xmlDocGetRootElement(c_obj());
    if (!node)
      data = object();
    else
      data = node::create(node);
    break;
  default: break;
  }
}

