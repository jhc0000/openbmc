#include <cstdio>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <libpldm/base.h>
#include <libpldm-oem/pldm.h>
#include <openbmc/obmc-i2c.h>
#include <openbmc/pal.h>
#include <openbmc/kv.h>
#include <openbmc/vr.h>
#include <syslog.h>
#include "swb_common.hpp"

using namespace std;
//MEB CXL VR Component
class CxlVrComponent : public VrComponent {
  protected:
    std::string name;
    uint8_t bus, eid, target;
    int update_proc(string image, bool force);
  public:
    CxlVrComponent(const string &fru, const string &comp, const string &dev_name, uint8_t dev_bus, uint8_t dev_eid, uint8_t dev_target)
        :VrComponent(fru, comp, dev_name), name(dev_name),  bus(dev_bus), eid(dev_eid), target(dev_target) {}
    int fupdate(string image);
    int update(string image);
    int get_version(json& j) override;
};

struct fruid_map {
  uint8_t fru_id;
  string fru_name;
};

struct fruid_map fru_map[] = {
  {FRU_MEB_JCN1,  "mc_cxl8"},
  {FRU_MEB_JCN2,  "mc_cxl7"},
  {FRU_MEB_JCN3,  "mc_cxl6"},
  {FRU_MEB_JCN4,  "mc_cxl5"},
  {FRU_MEB_JCN9,  "mc_cxl3"},
  {FRU_MEB_JCN10, "mc_cxl4"},
  {FRU_MEB_JCN11, "mc_cxl1"},
  {FRU_MEB_JCN12, "mc_cxl2"},
};

static int
vr_master_wr_pre(const string &fru) {
  fru_status status = {0, 0};
  uint8_t txbuf[MAX_TXBUF_SIZE] = {0};
  uint8_t rxbuf[MAX_RXBUF_SIZE] = {0};
  uint8_t offset = 0;
  uint8_t rxlen = 0;
  uint8_t txlen = 0;
  size_t rlen = 0;
  size_t channel_idx = 0;
  int ret = 0;
  uint16_t vr_timeout = 60; // secs

  auto fru_it = find_if(begin(fru_map), end(fru_map), [fru](auto x) {
    return x.fru_name == fru;
  });
  if (fru_it == end(fru_map))
    return -1;

  if ((pal_get_pldm_fru_status(fru_it->fru_id, JCN_0_1, &status) != 0) ||
      (status.fru_prsnt != FRU_PRSNT) || (status.fru_type != CXL_CARD)) {
    return -1;
  }

  // set FW update ongoing
  if (pal_set_fw_update_ongoing(fru_it->fru_id, vr_timeout) < 0) {
    return -1;
  }

  // wait sensor polling stopped
  sleep(2);

  channel_idx = distance(begin(fru_map), fru_it);

  // Switch MUX to CXL
  txbuf[0] = CXL_VR_BUS_ID; // Bus
  txbuf[1] = 0xE0; // Addr
  txbuf[2] = rxlen;
  txbuf[3] = offset;
  txlen += 4;

  uint8_t channel = 0x01;
  // Set Channel BIT due to fru
  txbuf[4] = (channel << channel_idx);
  txlen += 1;

  ret = oem_pldm_ipmi_send_recv(MEB_BIC_BUS, MEB_BIC_EID,
                               NETFN_APP_REQ, CMD_APP_MASTER_WRITE_READ,
                               txbuf, txlen,
                               rxbuf, &rlen,
                   true);
  if ( ret < 0 ) {
    cout << "ERROR: Switch MUX to CXL failed!" << endl;
    return ret;
  }

  // Switch MUX to VR
  txbuf[0] = CXL_VR_BUS_ID; // Bus
  txbuf[1] = 0xE2; // Addr
  txbuf[2] = rxlen;
  txbuf[3] = offset;
  txbuf[4] = 0x08; // Channel to VR
  ret = oem_pldm_ipmi_send_recv(MEB_BIC_BUS, MEB_BIC_EID,
                               NETFN_APP_REQ, CMD_APP_MASTER_WRITE_READ,
                               txbuf, txlen,
                   rxbuf, &rlen,
                   true);
  if ( ret < 0 ) {
    cout << "ERROR: Switch MUX to VR failed!" << endl;
    return ret;
  }

  return ret;
}

int CxlVrComponent::update_proc(string image, bool force) {
  int ret = 0;
  string comp = this->component();
  string fru  = this->fru();

  if(vr_master_wr_pre(fru) < 0) {
    cout << "VR Switch MUX failed" << endl;
    return -1;
  }

  if (vr_probe() < 0) {
    cout << "VR probe failed!" << endl;
    return -1;
  }
  syslog(LOG_CRIT, "Component %s upgrade initiated", comp.c_str());

  ret = vr_fw_update(name.c_str(), (char *)image.c_str(), force);
  if (ret < 0) {
    cout << "ERROR: VR Firmware update failed!" << endl;
  } else {
    syslog(LOG_CRIT, "Component %s %s completed", comp.c_str(), force? "force upgrade": "upgrade");
  }

  vr_remove();
  return ret;
}

int CxlVrComponent::update(string image) {
  int ret = 0;
  ret = update_proc(image, false);
  return ret;
}

int CxlVrComponent::fupdate(string image) {
  int ret = 0;
  ret = update_proc(image, true);
  return ret;
}

//Print Version
int get_cxl_vr_version (uint8_t bus, uint8_t eid, uint8_t target, vector<uint8_t> &data) {
  uint8_t tbuf[MAX_TXBUF_SIZE] = {0};
  uint8_t rbuf[MAX_RXBUF_SIZE] = {0};
  uint8_t tlen=0;
  size_t rlen = 0;
  int rc;

  tbuf[tlen++] = target;
  rc = oem_pldm_ipmi_send_recv(bus, eid,
                               NETFN_OEM_1S_REQ, CMD_OEM_1S_GET_FW_VER,
                               tbuf, tlen,
                               rbuf, &rlen, true);
  if (rc == 0)
    data = vector<uint8_t>(rbuf, rbuf + rlen);
  return rc;
}

int CxlVrComponent::get_version(json& j) {
  vector<uint8_t> ver = {};

  j["PRETTY_COMPONENT"] = name;

  // Get CXL VR Version
  int ret = get_cxl_vr_version(bus, eid, target, ver);
  if (ret != 0) {
    j["VERSION"] = "NA";
    return FW_STATUS_FAILURE;
  }

  string vendor_str;
  string tmp_str;
  string rw_str;

  switch(ver[5]) {
    case VR_RENESAS:
      vendor_str = "Renesas";
      break;
    case VR_INFINEON:
      vendor_str = "Infineon";
      break;
    case VR_VISHAY:
      vendor_str = "Vishay";
      break;
    case VR_ONSEMI:
       vendor_str = "Onsemi";
      break;
    default:
       vendor_str = "Unknown Vendor";
      break;
  }
  j["VENDOR"] = vendor_str;

  stringstream ver_str;
  ver_str << std::hex << std::setfill('0')
    << std::setw(2) << +ver[0]
    << std::setw(2) << +ver[1]
    << std::setw(2) << +ver[2]
    << std::setw(2) << +ver[3];
  tmp_str = ver_str.str();
  j["VERSION"] = tmp_str;

  ver_str.str("");
  ver_str.clear();
  ver_str << std::dec <<std::setw(2) << +ver[4];
  rw_str = ver_str.str();
  j["REMAINING_WRITE"] = rw_str;

  j["PRETTY_VERSION"] = vendor_str + " " + tmp_str + ", Remaining Write: " + rw_str;
  j["VERSION_ACTIVE"] = string(j["VERSION"]);

  return FW_STATUS_SUCCESS;
}

namespace pldm_signed_info
{
GTSwbBicFwComponent acb_bic("cb", "bic", ACB_BIC_BUS, ACB_BIC_EID, BIC_COMP,
    signed_header_t(gt_swb_comps, BIC_COMP, ASPEED));
GTSwbBicFwComponent meb_bic("mc", "bic", MEB_BIC_BUS, MEB_BIC_EID, BIC_COMP,
    signed_header_t(gt_swb_comps, BIC_COMP, ASPEED));
AcbPeswFwComponent acb_pesw0("cb", "pesw0", ACB_BIC_BUS, ACB_BIC_EID, PEX0_COMP);
AcbPeswFwComponent acb_pesw1("cb", "pesw1", ACB_BIC_BUS, ACB_BIC_EID, PEX1_COMP);
}

MebCxlFwComponent meb_cxl1("mc", "cxl1", MEB_BIC_BUS, MEB_BIC_EID, CXL1_COMP);
MebCxlFwComponent meb_cxl2("mc", "cxl2", MEB_BIC_BUS, MEB_BIC_EID, CXL2_COMP);
MebCxlFwComponent meb_cxl3("mc", "cxl3", MEB_BIC_BUS, MEB_BIC_EID, CXL3_COMP);
MebCxlFwComponent meb_cxl4("mc", "cxl4", MEB_BIC_BUS, MEB_BIC_EID, CXL4_COMP);
MebCxlFwComponent meb_cxl5("mc", "cxl5", MEB_BIC_BUS, MEB_BIC_EID, CXL5_COMP);
MebCxlFwComponent meb_cxl6("mc", "cxl6", MEB_BIC_BUS, MEB_BIC_EID, CXL6_COMP);
MebCxlFwComponent meb_cxl7("mc", "cxl7", MEB_BIC_BUS, MEB_BIC_EID, CXL7_COMP);
MebCxlFwComponent meb_cxl8("mc", "cxl8", MEB_BIC_BUS, MEB_BIC_EID, CXL8_COMP);

SwbVrComponent vr_pesw_vcc("cb", "pesw_vr", "VR_PESW_VCC");

CxlVrComponent vr_jcn11_a0v8_9 ("mc_cxl1", "vr_p0v89a",         "MC CXL1 VR_P0V89A", MEB_BIC_BUS, MEB_BIC_EID, CXL1_VR_P089A_COMP);
CxlVrComponent vr_jcn11_vddq_ab("mc_cxl1", "vr_p0v8d_pvddq_ab", "MC CXL1 VR_P0V8D_PVDDQ_AB", MEB_BIC_BUS, MEB_BIC_EID, CXL1_VR_P0V8D_PVDDQ_AB_COMP);
CxlVrComponent vr_jcn11_vddq_cd("mc_cxl1", "vr_pvddq_cd",       "MC CXL1 VR_PVDDQ_CD", MEB_BIC_BUS, MEB_BIC_EID, CXL1_VR_PVDDQ_CD_COMP);
CxlVrComponent vr_jcn12_a0v8_9 ("mc_cxl2", "vr_p0v89a",         "MC CXL2 VR_P0V89A", MEB_BIC_BUS, MEB_BIC_EID, CXL2_VR_P089A_COMP);
CxlVrComponent vr_jcn12_vddq_ab("mc_cxl2", "vr_p0v8d_pvddq_ab", "MC CXL2 VR_P0V8D_PVDDQ_AB", MEB_BIC_BUS, MEB_BIC_EID, CXL2_VR_P0V8D_PVDDQ_AB_COMP);
CxlVrComponent vr_jcn12_vddq_cd("mc_cxl2", "vr_pvddq_cd",       "MC CXL2 VR_PVDDQ_CD", MEB_BIC_BUS, MEB_BIC_EID, CXL2_VR_PVDDQ_CD_COMP);
CxlVrComponent vr_jcn9_a0v8_9  ("mc_cxl3", "vr_p0v89a",         "MC CXL3 VR_P0V89A", MEB_BIC_BUS, MEB_BIC_EID, CXL3_VR_P089A_COMP);
CxlVrComponent vr_jcn9_vddq_ab ("mc_cxl3", "vr_p0v8d_pvddq_ab", "MC CXL3 VR_P0V8D_PVDDQ_AB", MEB_BIC_BUS, MEB_BIC_EID, CXL3_VR_P0V8D_PVDDQ_AB_COMP);
CxlVrComponent vr_jcn9_vddq_cd ("mc_cxl3", "vr_pvddq_cd",       "MC CXL3 VR_PVDDQ_CD", MEB_BIC_BUS, MEB_BIC_EID, CXL3_VR_PVDDQ_CD_COMP);
CxlVrComponent vr_jcn10_a0v8_9 ("mc_cxl4", "vr_p0v89a",         "MC CXL4 VR_P0V89A", MEB_BIC_BUS, MEB_BIC_EID, CXL4_VR_P089A_COMP);
CxlVrComponent vr_jcn10_vddq_ab("mc_cxl4", "vr_p0v8d_pvddq_ab", "MC CXL4 VR_P0V8D_PVDDQ_AB", MEB_BIC_BUS, MEB_BIC_EID, CXL4_VR_P0V8D_PVDDQ_AB_COMP);
CxlVrComponent vr_jcn10_vddq_cd("mc_cxl4", "vr_pvddq_cd",       "MC CXL4 VR_PVDDQ_CD", MEB_BIC_BUS, MEB_BIC_EID, CXL4_VR_PVDDQ_CD_COMP);
CxlVrComponent vr_jcn4_a0v8_9  ("mc_cxl5", "vr_p0v89a",         "MC CXL5 VR_P0V89A", MEB_BIC_BUS, MEB_BIC_EID, CXL5_VR_P089A_COMP);
CxlVrComponent vr_jcn4_vddq_ab ("mc_cxl5", "vr_p0v8d_pvddq_ab", "MC CXL5 VR_P0V8D_PVDDQ_AB", MEB_BIC_BUS, MEB_BIC_EID, CXL5_VR_P0V8D_PVDDQ_AB_COMP);
CxlVrComponent vr_jcn4_vddq_cd ("mc_cxl5", "vr_pvddq_cd",       "MC CXL5 VR_PVDDQ_CD", MEB_BIC_BUS, MEB_BIC_EID, CXL5_VR_PVDDQ_CD_COMP);
CxlVrComponent vr_jcn3_a0v8_9  ("mc_cxl6", "vr_p0v89a",         "MC CXL6 VR_P0V89A", MEB_BIC_BUS, MEB_BIC_EID, CXL6_VR_P089A_COMP);
CxlVrComponent vr_jcn3_vddq_ab ("mc_cxl6", "vr_p0v8d_pvddq_ab", "MC CXL6 VR_P0V8D_PVDDQ_AB", MEB_BIC_BUS, MEB_BIC_EID, CXL6_VR_P0V8D_PVDDQ_AB_COMP);
CxlVrComponent vr_jcn3_vddq_cd ("mc_cxl6", "vr_pvddq_cd",       "MC CXL6 VR_PVDDQ_CD", MEB_BIC_BUS, MEB_BIC_EID, CXL6_VR_PVDDQ_CD_COMP);
CxlVrComponent vr_jcn2_a0v8_9  ("mc_cxl7", "vr_p0v89a",         "MC CXL7 VR_P0V89A", MEB_BIC_BUS, MEB_BIC_EID, CXL7_VR_P089A_COMP);
CxlVrComponent vr_jcn2_vddq_ab ("mc_cxl7", "vr_p0v8d_pvddq_ab", "MC CXL7 VR_P0V8D_PVDDQ_AB", MEB_BIC_BUS, MEB_BIC_EID, CXL7_VR_P0V8D_PVDDQ_AB_COMP);
CxlVrComponent vr_jcn2_vddq_cd ("mc_cxl7", "vr_pvddq_cd",       "MC CXL7 VR_PVDDQ_CD", MEB_BIC_BUS, MEB_BIC_EID, CXL7_VR_PVDDQ_CD_COMP);
CxlVrComponent vr_jcn1_a0v8_9  ("mc_cxl8", "vr_p0v89a",         "MC CXL8 VR_P0V89A", MEB_BIC_BUS, MEB_BIC_EID, CXL8_VR_P089A_COMP);
CxlVrComponent vr_jcn1_vddq_ab ("mc_cxl8", "vr_p0v8d_pvddq_ab", "MC CXL8 VR_P0V8D_PVDDQ_AB", MEB_BIC_BUS, MEB_BIC_EID, CXL8_VR_P0V8D_PVDDQ_AB_COMP);
CxlVrComponent vr_jcn1_vddq_cd ("mc_cxl8", "vr_pvddq_cd",       "MC CXL8 VR_PVDDQ_CD", MEB_BIC_BUS, MEB_BIC_EID, CXL8_VR_PVDDQ_CD_COMP);
