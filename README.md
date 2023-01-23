# im_data

im_data is an immediate mode library which allows to define data for custom types and process them with various backends. 

Similar to how you can define graphical interfaces in imgui without building a separate DOM (document object model), 
you define data structures for your own types by just writing functions.
Templates are used for fast optimized code, eliminating the call overhead to the various calls which define the data structure.

Included are backends for automatic imgui parameter gui and json de-/serializers.
