# EngFlow
Dataflow suite specialised for arbitrary code execution and live results. Currently unstable, breaking changes frequent. 
Probable change of name will be required due to existing products.

# Design Goals
Node based dataflow editing
Cross platform (assuming prerequisites are installed)
Plugins
Codeblocks of any language
External language bindings

# Language Blocks Progress
- Python (3.8)
  - [x] Binding (Python)
  - [x] Runtime (Python)
  - [x] Reflection (inspect.signature)
- Lua (5.3)
  - [x] Binding ([Sol2](https://github.com/ThePhD/sol2))
  - [x] Runtime (Lua)
  - [ ] Reflection (???)
- Javascript
  - [ ] Binding (V8)
  - [ ] Runtime (V8)
  - [ ] Reflection (???)
- Java
  - [ ] Binding (JNI)
  - [ ] Runtime (JNI)
  - [ ] Reflection (???)
- MATLAB
  - [ ] Binding (mex)
  - [ ] Runtime (???)
  - [ ] Reflection (???)
- Maple
  - [ ] Binding (OpenMaple)
  - [ ] Runtime (OpenMaple)
  - [ ] Reflection (Builtin Decomposition)
- C/C++
  - [x] Binding (native)
  - [ ] Runtime (???)
  - [ ] Reflection (??????)
