
aux_source_directory(. SourceFiles)
add_executable(chill-parser-test ${SourceFiles})
target_link_libraries(chill-parser-test ChillParserLexer)
