# im_data

im_data is an immediate mode library which allows to define data for custom types and process them with various backends. 
similar to how you can define graphical interfaces in imgui without building a separate DOM (document object model), 
you define data structures for your own types by just writing functions.
templates are used for fast optimized code, eliminating the call overhead to the various calls which define the data structure.
imgui and json serializer backend provided.
