aux_source_directory(. SRC_LIST)
aux_source_directory(./controller/src CONTROLLER_SRC_LIST)
aux_source_directory(./entity/src ENTITY_SRC_LIST)
aux_source_directory(./mapping/src MAPPING_SRC_LIST)
aux_source_directory(./service/src SERVICE_SRC_LIST)
aux_source_directory(./util/src UTIL_SRC_LIST)

add_executable(server ${SRC_LIST} ${CONTROLLER_SRC_LIST} ${ENTITY_SRC_LIST} ${MAPPING_SRC_LIST} ${SERVICE_SRC_LIST} ${UTIL_SRC_LIST})

target_link_libraries(server webserver)
