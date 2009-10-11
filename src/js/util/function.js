// vim:ts=2:sw=2:expandtab:autoindent:
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

/**
 * Bind a function to an object.
 *
 * @param {Object} obj The object to bind "this" to.
 *
 * @example
 * function myfunc() { ... }
 * bound = myfunc.bind(myobj)
 * bound() // calls myfunc with myobj as "this"
 *
 * @name Function.prototype.bind
 */
Function.prototype.bind = function bind(obj) {
  var fun = this;
  return function bound_func() {
      return fun.apply(obj, arguments);
    }
};

/**
 * Bind an object method to its object so that it can be called without the
 * object.
 *
 * @param {Object} obj   The object that contains the method and also the object
 *                       to bind the method to.
 * @param {string} name  The method name.
 *
 * @name Function.bind
 */
Function.bind = function bind(obj, name) {
  var fun = obj[name];
  if (!fun || !(fun instanceof Function))
    throw new Error("Object has no function '" + name + "'");
  return fun.bind(obj);
}

Object.defineProperty(Function.prototype, 'bind', { enumerable: false });
Object.defineProperty(Function, 'bind', { enumerable: false });