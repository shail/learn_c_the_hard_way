How the CPP Works
- C Pre-Processor is a template processing system. Traditionally it has been used for storing constants and
  making macros. In modern C, you will actually use the CPP as a code generator to create templated pieces of
  code. You provide the CPP one file, and it processess the text starting with the # character. When it
  encounters one of these it performs a specific replacement on the text of the input file. The main advantage is that it can include other files, and then augment its list of macros based on that file's content.
