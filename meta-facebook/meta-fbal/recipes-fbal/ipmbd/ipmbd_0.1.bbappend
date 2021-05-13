# Copyright 2015-present Facebook. All Rights Reserved.

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI += " file://setup-ipmbd.sh \
             file://run-ipmbd_0.sh \
             file://run-ipmbd_2.sh \
             file://run-ipmbd_5.sh \
             file://run-ipmbd_6.sh \
             file://run-ipmbd_8.sh \
           "
DEPENDS_append = " plat-utils update-rc.d-native"

do_install() {
  dst="${D}/usr/local/fbpackages/${pkgdir}"
  bin="${D}/usr/local/bin"
  install -d $dst
  install -d $bin
  install -m 755 ipmbd ${dst}/ipmbd
  ln -snf ../fbpackages/${pkgdir}/ipmbd ${bin}/ipmbd
  install -d ${D}${sysconfdir}/init.d
  install -d ${D}${sysconfdir}/rcS.d
  install -d ${D}${sysconfdir}/sv
  install -d ${D}${sysconfdir}/sv/ipmbd_0
  install -d ${D}${sysconfdir}/sv/ipmbd_2
  install -d ${D}${sysconfdir}/sv/ipmbd_5
  install -d ${D}${sysconfdir}/sv/ipmbd_6
  install -d ${D}${sysconfdir}/sv/ipmbd_8
  install -m 755 setup-ipmbd.sh ${D}${sysconfdir}/init.d/setup-ipmbd.sh
  install -m 755 run-ipmbd_0.sh ${D}${sysconfdir}/sv/ipmbd_0/run
  install -m 755 run-ipmbd_2.sh ${D}${sysconfdir}/sv/ipmbd_2/run
  install -m 755 run-ipmbd_5.sh ${D}${sysconfdir}/sv/ipmbd_5/run
  install -m 755 run-ipmbd_6.sh ${D}${sysconfdir}/sv/ipmbd_6/run
  install -m 755 run-ipmbd_8.sh ${D}${sysconfdir}/sv/ipmbd_8/run
  update-rc.d -r ${D} setup-ipmbd.sh start 65 5 .
}

FBPACKAGEDIR = "${prefix}/local/fbpackages"

FILES_${PN} = "${FBPACKAGEDIR}/ipmbd ${prefix}/local/bin ${sysconfdir} "

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
