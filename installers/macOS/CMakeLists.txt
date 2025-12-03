# macOS installer configuration
set(CPACK_GENERATOR "productbuild")
set(CPACK_PACKAGE_FILE_NAME "AuricHalo-${PROJECT_VERSION}-macOS")

# Post-install script
install(PROGRAMS auric_halo_pkg_postinstall.sh
        DESTINATION .
        RENAME postinstall)
