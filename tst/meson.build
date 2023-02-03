quadstore_inserts_test = executable('quadstore_inserts_test', 'rdf-store/test_quadstore_insert.cc',
                    include_directories : inc, dependencies: [catch], link_with: [rdfstore])

utils_test = executable('utils_test', 'rdf-store/test_utils.cc',
                    include_directories : inc, dependencies: [catch], link_with: [rdfstore])

test('RDF store inserts test', quadstore_inserts_test)
test('Utils test', utils_test)