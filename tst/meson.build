rdf_store_inserts_test = executable('rdf_store_inserts_test', 'rdf-store/test_rdf_store_insert.cc',
                    include_directories : inc, dependencies: [catch], link_with: [rdfstore])

utils_test = executable('utils_test', 'rdf-store/test_utils.cc',
                    include_directories : inc, dependencies: [catch])

test('RDF store inserts test', rdf_store_inserts_test)
test('Utils test', utils_test)