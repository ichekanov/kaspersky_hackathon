project (einit)

# Tools for working with KasperskyOS images.
include (platform/image)

set (ENTITIES Client)
set (SECURITY_PSL_FILE "src/security.psl.in")

build_kos_qemu_image (kos-qemu-image
                      EINIT_ENTITY EinitQemu
                      CONNECTIONS_CFG "src/init.yaml.in"
                      SECURITY_PSL ${SECURITY_PSL_FILE}
                      IMAGE_FILES ${ENTITIES})

# KasperskyOS image for the target hardware platform.
build_kos_hw_image (kos-image
                    EINIT_ENTITY EinitHw
                    CONNECTIONS_CFG "src/init.yaml.in"
                    SECURITY_PSL ${SECURITY_PSL_FILE}
                    IMAGE_FILES ${ENTITIES})

set_target_properties (EinitQemu PROPERTIES LINK_FLAGS ${VULN_LDFLAGS})
set_target_properties (EinitHw PROPERTIES LINK_FLAGS ${VULN_LDFLAGS})
