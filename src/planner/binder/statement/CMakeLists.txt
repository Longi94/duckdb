add_library_unity(duckdb_bind_statement
                  OBJECT
                  bind_copy.cpp
                  bind_create_index.cpp
                  bind_create_table.cpp
                  bind_create_view.cpp
                  bind_delete.cpp
                  bind_execute.cpp
                  bind_explain.cpp
                  bind_insert.cpp
                  bind_select.cpp
                  bind_simple.cpp
                  bind_update.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:duckdb_bind_statement>
    PARENT_SCOPE)
