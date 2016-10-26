# PluginX
Plugin eXercises
======
Exercises on plugin dynamic loading usig **Boost.DLL library**.
Dependencies: only the Boost library.
Feel free to use it as it provides a good working understanding of how dynamic plugin linking is done.

1. Finding files with ".dll" extension
  + using Boost.Filesystem
2. Loading dlls into *boost::dll::shared_lib* class
3. Obtaining pointer to factory function from each lib.
4. Creating an instance of each plugin
5. Using a plugin through a polymorphic pointer to *plugin_ifc* 
