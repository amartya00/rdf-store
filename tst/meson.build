rdf_store_inserts_test = executable('rdf_store_inserts_test', 'rdf-store/test_rdf_store_insert.cc',
                    include_directories : inc, dependencies: [catch], link_with: [rdfstore])

rdf_store_query_test = executable('rdf_store_query_test', 'rdf-store/test_rdf_store_query.cc',
                    include_directories : inc, dependencies: [catch], link_with: [rdfstore])

utils_test = executable('utils_test', 'rdf-store/test_utils.cc',
                    include_directories : inc, dependencies: [catch], link_with: [rdfstore])

test('RDF store inserts test', rdf_store_inserts_test)
test('RDF store query test', rdf_store_query_test)
test('Utils test', utils_test)