# Copyright 2022-present Facebook. All Rights Reserved.
#
# This program file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program in a file named COPYING; if not, write to the
# Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
CFLAGS += "-DSENSOR_FAIL_DETECT"
SENSORD_MONITORED_FRUS = "mb nic0 nic1 vpdb hpdb fan_bp0 fan_bp1 scm acb meb meb_jcn1 meb_jcn2 meb_jcn3 meb_jcn4 meb_jcn9 meb_jcn10 meb_jcn11 meb_jcn12 meb_jcn13 meb_jcn14 "
