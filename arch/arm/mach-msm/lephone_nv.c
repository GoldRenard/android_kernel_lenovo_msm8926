
/*
  Tony Sun : Add for Get nv data from modem using SMEM.
*/

#include <linux/types.h>
#include <linux/sysdev.h>
#include <linux/proc_fs.h>
#include "smd_private.h"
#include "include/mach/proc_comm.h"

#define NV_WIFI_ADDR_SIZE	6
#define NV_BT_ADDR_SIZE		6
#define NV_MAX_SIZE		512
/* [dongxq1 merged BEGIN] guohh1 20131011 add for FACTORYDATACHECK */
//#define NV_OTHERS_SIZE	(NV_MAX_SIZE - NV_WIFI_ADDR_SIZE - NV_BT_ADDR_SIZE)
#define NV_OTHERS_SIZE   (NV_MAX_SIZE - NV_WIFI_ADDR_SIZE - NV_BT_ADDR_SIZE-32-32-32-16-16-16-16-32)
/* [dongxq1 merged END   ] guohh1 20131011 add for FACTORYDATACHECK*/

struct smem_nv {
	unsigned char nv_wifi[NV_WIFI_ADDR_SIZE];
	unsigned char nv_bt[NV_BT_ADDR_SIZE];
       /* [dongxq1 merged BEGIN] guohh1 20131011 add for FACTORYDATACHECK */
       unsigned char nv_sn1[32];
       unsigned char nv_sn2[32];
       unsigned char nv_meid[16];
       unsigned char nv_imei1[16];
       unsigned char nv_imei2[16];
       unsigned char nv_hwid[16];
       unsigned char nv_station[32];
       /* [dognxq1 merged END   ] guohh1 20131011 add for FACTORYDATACHECK*/
	   
	   /* [qiaoyf1 BEGIN] qiaoyf1 20140102 add for FACTORYDATACHECK */
	  unsigned char nv_flow[32];
     /* [qiaoyf1 END] qiaoyf1 20140102 add for FACTORYDATACHECK */
	  unsigned char nv_policyman[2];
       unsigned char nv_others[NV_OTHERS_SIZE];
};
static struct smem_nv * psmem_nv = NULL;



static struct smem_nv * smem_read_nv(void)
{
	struct smem_nv * buf;
	buf = smem_alloc(SMEM_ID_VENDOR_READ_NV, NV_MAX_SIZE);
	if(!buf)
		printk(KERN_ERR "SMEM_ID_VENDOR_READ_NV smem_alloc failed\n");
	return buf;
}

static int dump_wifi_addr(char *buf, char **start, off_t offset,
                  int count, int *eof, void *data)
{
	int len = 0;
	if (!psmem_nv)
		psmem_nv = smem_read_nv();
	if (!psmem_nv)
		return 0;

	printk(KERN_ERR "wifi addr  = 0x %02x %02x %02x %02x %02x %02x\n",
		psmem_nv->nv_wifi[0],psmem_nv->nv_wifi[1],psmem_nv->nv_wifi[2],
		psmem_nv->nv_wifi[3],psmem_nv->nv_wifi[4],psmem_nv->nv_wifi[5]);
        memcpy( buf, psmem_nv->nv_wifi, NV_WIFI_ADDR_SIZE);
        len = NV_WIFI_ADDR_SIZE;
	*eof = 1;
	return len;
}

int wlan_get_nv_mac(char* buf)
{
	int ret = -1;
        if (!psmem_nv) {
		psmem_nv = smem_read_nv();
	}

	if (!psmem_nv){
		printk(KERN_ERR "Could not get smem for wlan mac nv\n");
	        return ret;
	}

        printk(KERN_ERR "wifi addr  = 0x %02x %02x %02x %02x %02x %02x\n",
                psmem_nv->nv_wifi[0],psmem_nv->nv_wifi[1],psmem_nv->nv_wifi[2],
                psmem_nv->nv_wifi[3],psmem_nv->nv_wifi[4],psmem_nv->nv_wifi[5]);
        memcpy( buf, psmem_nv->nv_wifi, NV_WIFI_ADDR_SIZE);
	return 0;
}
EXPORT_SYMBOL_GPL(wlan_get_nv_mac);

static int dump_bt_addr(char *buf, char **start, off_t offset,
                  int count, int *eof, void *data)
{
	int len = 0;
	if (!psmem_nv)
		psmem_nv = smem_read_nv();
	if (!psmem_nv)
		return 0;

	printk(KERN_ERR "bt addr  = 0x %02x %02x %02x %02x %02x %02x\n",
		psmem_nv->nv_bt[0],psmem_nv->nv_bt[1],psmem_nv->nv_bt[2],
		psmem_nv->nv_bt[3],psmem_nv->nv_bt[4],psmem_nv->nv_bt[5]);
	memcpy( buf, psmem_nv->nv_bt, NV_BT_ADDR_SIZE);
	len = NV_BT_ADDR_SIZE;
	*eof = 1;
	return len;
}
/* [dongxq1 merged BEGIN] guohh1 20131011 add for FACTORYDATACHECK */
static int dump_lnv_sn1(char *buf, char **start, off_t offset,
                  int count, int *eof, void *data)
{
    int len = 0;
    if (!psmem_nv)
        psmem_nv = smem_read_nv();
    if (!psmem_nv)
        return 0;
    memcpy( buf, psmem_nv->nv_sn1, 32);
    len = strlen(psmem_nv->nv_sn1);
    *eof = 1;
    return len;
}
static int dump_lnv_sn2(char *buf, char **start, off_t offset,
                  int count, int *eof, void *data)
{
    int len = 0;
    if (!psmem_nv)
        psmem_nv = smem_read_nv();
    if (!psmem_nv)
        return 0;
    memcpy( buf, psmem_nv->nv_sn2, 32);
    len = strlen(psmem_nv->nv_sn2);
    *eof = 1;
    return len;
}
static int dump_lnv_meid(char *buf, char **start, off_t offset,
                  int count, int *eof, void *data)
{
    int len = 0;
    if (!psmem_nv)
        psmem_nv = smem_read_nv();
    if (!psmem_nv)
        return 0;

    memcpy( buf, psmem_nv->nv_meid, 16);
    len = strlen(psmem_nv->nv_meid);
    *eof = 1;
    return len;
}
static int dump_lnv_imei1(char *buf, char **start, off_t offset,
                  int count, int *eof, void *data)
{
    int len = 0;
    if (!psmem_nv)
        psmem_nv = smem_read_nv();
    if (!psmem_nv)
        return 0;

    memcpy( buf, psmem_nv->nv_imei1, 16);
    len = strlen(psmem_nv->nv_imei1);
    *eof = 1;
    return len;
}
static int dump_lnv_imei2(char *buf, char **start, off_t offset,
                  int count, int *eof, void *data)
{
    int len = 0;
    if (!psmem_nv)
        psmem_nv = smem_read_nv();
    if (!psmem_nv)
        return 0;

    memcpy( buf, psmem_nv->nv_imei2, 16);
    len = strlen(psmem_nv->nv_imei2);
    *eof = 1;
    return len;
}
static int dump_lnv_hwid(char *buf, char **start, off_t offset,
                  int count, int *eof, void *data)
{
    int len = 0;
    if (!psmem_nv)
        psmem_nv = smem_read_nv();
    if (!psmem_nv)
        return 0;
    memcpy( buf, psmem_nv->nv_hwid, 16);
    len = strlen(psmem_nv->nv_hwid);
    *eof = 1;
    return len;
}
static int dump_lnv_station(char *buf, char **start, off_t offset,
                  int count, int *eof, void *data)
{
    int len = 0;
    if (!psmem_nv)
        psmem_nv = smem_read_nv();
    if (!psmem_nv)
        return 0;
    memcpy( buf, psmem_nv->nv_station, 32);
    len = strlen(psmem_nv->nv_station);
    *eof = 1;
    return len;
}
/* [dongxq1 merged END   ] guohh1 20131011 add for FACTORYDATACHECK*/
/* [qiaoyf1 BEGIN] qiaoyf1 20140102 add for FACTORYDATACHECK */
static int dump_lnv_flow(char *buf, char **start, off_t offset,
                  int count, int *eof, void *data)
{
    int len = 0;
    if (!psmem_nv)
        psmem_nv = smem_read_nv();
    if (!psmem_nv)
        return 0;
    memcpy( buf, psmem_nv->nv_flow, 32);
    len = strlen(psmem_nv->nv_flow);
    *eof = 1;
    return len;
}
/* [qiaoyf1 END] qiaoyf1 20140102 add for FACTORYDATACHECK */

static int dump_policyman(char *buf, char **start, off_t offset,
                  int count, int *eof, void *data)
{
    int len = 0;
    if (!psmem_nv)
        psmem_nv = smem_read_nv();
    if (!psmem_nv)
        return 0;
    memcpy( buf, psmem_nv->nv_policyman, 2);
    len = strlen(psmem_nv->nv_policyman);
    *eof = 1;
    return len;
}
static void show_nv(void)
{
	struct proc_dir_entry *wifi_addr_entry;
	struct proc_dir_entry *bt_addr_entry;
       /* [dongxq1 mergd BEGIN] guohh1 20131011 add for FACTORYDATACHECK */
       struct proc_dir_entry *sn1_addr_entry;
       struct proc_dir_entry *sn2_addr_entry;
       struct proc_dir_entry *meid_addr_entry;
       struct proc_dir_entry *imei1_addr_entry;
       struct proc_dir_entry *imei2_addr_entry;
       struct proc_dir_entry *hwid_addr_entry;
       struct proc_dir_entry *station_addr_entry;
       /* [dongxq1 merged END   ] guohh1 20131011 add for FACTORYDATACHECK*/
	   /* [qiaoyf1 BEGIN] qiaoyf1 20140102 add for FACTORYDATACHECK */
	  struct proc_dir_entry *flow_addr_entry;
     /* [qiaoyf1 END] qiaoyf1 20140102 add for FACTORYDATACHECK */
      struct proc_dir_entry *policyman_addr_entry;

	wifi_addr_entry = create_proc_entry("mac_wifi", 0, NULL);
	bt_addr_entry = create_proc_entry("mac_bt", 0, NULL);

	if (wifi_addr_entry)
		wifi_addr_entry ->read_proc = &dump_wifi_addr;

	if (bt_addr_entry)
		bt_addr_entry ->read_proc = &dump_bt_addr;
       /* [dongxq1 merged BEGIN] guohh1 20131011 add for FACTORYDATACHECK */
       sn1_addr_entry = create_proc_entry("lnvsn1", 0, NULL);
       if (sn1_addr_entry)
           sn1_addr_entry ->read_proc = &dump_lnv_sn1;
       sn2_addr_entry = create_proc_entry("lnvshowcode", 0, NULL);
       if (sn2_addr_entry)
           sn2_addr_entry ->read_proc = &dump_lnv_sn2;
       meid_addr_entry = create_proc_entry("lnvmeid", 0, NULL);
       if (meid_addr_entry)
           meid_addr_entry ->read_proc = &dump_lnv_meid;
       imei1_addr_entry = create_proc_entry("lnvimei1", 0, NULL);
       if (imei1_addr_entry)
           imei1_addr_entry ->read_proc = &dump_lnv_imei1;
       imei2_addr_entry = create_proc_entry("lnvimei2", 0, NULL);
       if (imei2_addr_entry)
           imei2_addr_entry ->read_proc = &dump_lnv_imei2;
       hwid_addr_entry = create_proc_entry("lnvhwid", 0, NULL);
       if (hwid_addr_entry)
       {
           hwid_addr_entry ->read_proc = &dump_lnv_hwid;
       }
       station_addr_entry = create_proc_entry("lnvstation", 0, NULL);
       if (station_addr_entry)
       {
           station_addr_entry ->read_proc = &dump_lnv_station;
       }
      /* [dongxq1 merged END   ] guohh1 20131011 add for FACTORYDATACHECK*/
	  /* [qiaoyf1 BEGIN] qiaoyf1 20140102 add for FACTORYDATACHECK */
	  flow_addr_entry = create_proc_entry("lnvflow", 0, NULL);
       if (flow_addr_entry)
           flow_addr_entry ->read_proc = &dump_lnv_flow;
     /* [qiaoyf1 END] qiaoyf1 20140102 add for FACTORYDATACHECK */
	  policyman_addr_entry = create_proc_entry("policyman", 0, NULL);
      if (policyman_addr_entry)
      {
           policyman_addr_entry ->read_proc = &dump_policyman;
      }
}

void __init lephone_nv_init(void)
{
	show_nv();
}

