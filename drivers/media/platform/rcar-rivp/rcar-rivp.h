#ifndef __RCAR_RIVP__
#define __RCAR_RIVP__

#include <linux/device.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/clk.h>
#include <linux/kref.h>
#include <linux/reset.h>

#include <media/v4l2-ioctl.h>
#include <media/v4l2-async.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-dev.h>
#include <media/v4l2-device.h>
#include <media/videobuf2-v4l2.h>
#include <media/videobuf2-core.h>



#define MAX_RIVP_DEVICE_NUM	8
# define DT_RCAR_RIVP_MEM_NODE "/rcar-rivp/rivp-memory"

#define rivp_dbg(d, fmt, arg...)       dev_dbg(d->dev, fmt, ##arg)
#define rivp_inf(d, fmt, arg...)       dev_info(d->dev, fmt, ##arg)
#define rivp_wrn(d, fmt, arg...)       dev_warn(d->dev, fmt, ##arg)
#define rivp_err(d, fmt, arg...)       dev_err(d->dev, fmt, ##arg)



struct rivp_device;

enum rivp_dma_state
{
   STOPPED = 0,
   RUNNING,
   STALLED,
   STOPPING,
};

struct rivp_v4l2_device
{
   struct device*             dev;
   struct rivp_device*        rivp_dev;
   struct video_device        video_dev;
   struct v4l2_device         v4l2_dev;
   int                        channel;
};

struct rivp_device
{
   struct device*             dev;
   struct rpmsg_device*       rpmsg_dev;
   int                        channel_num;
   struct rivp_v4l2_device*   rivp_v4l2_dev[ MAX_RIVP_DEVICE_NUM ];
};

enum codec_cmd_type
{
   CMD_CONFIGURE_CODEC,
   CMD_ENCODE_FRAME,
   CMD_FRAME_DONE,
   CMD_ERROR
};

struct rivp_msg
{
   u32 cmd;
};



#endif
