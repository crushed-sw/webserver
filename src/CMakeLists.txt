aux_source_directory(. SRC_LIST)

add_library(webserver ${SRC_LIST})

target_link_libraries(webserver mysqlcppconn)

