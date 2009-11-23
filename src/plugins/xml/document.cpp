// vim:ts=2:sw=2:expandtab:autoindent:filetype=cpp:
/*
The MIT License

Copyright (c) 2008, 2009 Flusspferd contributors (see "CONTRIBUTORS" or
                                       http://flusspferd.org/contributors.txt)

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

#include <flusspferd.hpp>
#include <flusspferd/aliases.hpp>

#include "document.hpp"
#include "node_list.hpp"

using namespace flusspferd;
using namespace flusspferd::aliases;
using namespace xml_plugin;

document::document(object const &proto, call_context &)
  : base_type(proto)
{
}


document::document(object const &proto, wrapped_type const &doc)
  : base_type(proto, doc),
    doc_(doc),
    non_shared_node_map_(node_map::make(*this, doc_))
{
}

document::~document() {
}

object document::getDocumentElement() {
  return non_shared_node_map_->get_node(doc_.getDocumentElement());
}

object document::getElementsByTagName(std::string tagname) {
  return create<node_list>( make_vector(
    doc_.getElementsByTagName(tagname),
    non_shared_node_map_
  ) );
}
