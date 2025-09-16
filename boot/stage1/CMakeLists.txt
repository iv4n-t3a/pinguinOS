add_executable(mbr.img mbr.S)
set_target_properties(
  mbr.img PROPERTIES LINK_DEPENDS "${PROJECT_SOURCE_DIR}/boot/stage1/mbr.ld")
target_link_options(mbr.img PRIVATE
                    "-T${PROJECT_SOURCE_DIR}/boot/stage1/mbr.ld")

add_executable(vbr.img vbr.S)
set_target_properties(
  vbr.img PROPERTIES LINK_DEPENDS "${PROJECT_SOURCE_DIR}/boot/stage1/vbr.ld")
target_link_options(vbr.img PRIVATE
                    "-T${PROJECT_SOURCE_DIR}/boot/stage1/vbr.ld")
