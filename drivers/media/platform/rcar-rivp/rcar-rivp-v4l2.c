#include <linux/pm_runtime.h>
#include <linux/delay.h>
#include <media/v4l2-event.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-mc.h>
#include <media/v4l2-rect.h>
#include <media/videobuf2-dma-contig.h>

#include "rcar-rivp.h"



// static const struct v4l2_m2m_ops m2m_ops = {
//    .device_run                   = device_run,
// };

// static const struct vb2_ops vb2_ops = {
//    .queue_setup                  = queue_setup,
//    .buf_prepare                  = buf_prepare,
//    .wait_prepare                 = vb2_ops_wait_prepare,
//    .wait_finish                  = vb2_ops_wait_finish,
// };



static int vidioc_querycap( struct file* file, void* priv, struct v4l2_capability* cap )
{
   struct rivp_v4l2_device* rivp_v4l2_dev = video_drvdata( file );

   strlcpy( cap->driver, KBUILD_MODNAME, sizeof(cap->driver) );
   strlcpy( cap->card, "R_Car_RIVP", sizeof(cap->card) );
   snprintf( cap->bus_info, sizeof(cap->bus_info), "platform:%s", dev_name( rivp_v4l2_dev->dev ) );
   cap->capabilities = V4L2_CAP_VIDEO_M2M | V4L2_CAP_STREAMING;

   return 0;
}

static const struct v4l2_ioctl_ops vcodec_ioctl_ops = {
   // // https://www.kernel.org/doc/html/v4.9/media/uapi/v4l/user-func.html
   // // https://codebrowser.dev/linux/linux/include/media/v4l2-ioctl.h.html
   // // linux/include/media/v4l2-ioctl.h

   // // Get or set the data format, try a format
   // VIDIOC_G_FMT
   // VIDIOC_S_FMT
   // VIDIOC_TRY_FMT

   // // Get or set the current cropping rectangle
   // VIDIOC_G_CROP
   // VIDIOC_S_CROP

   // // Get or set the value of a control
   // VIDIOC_G_CTRL
   // // VIDIOC_S_CTRL - no

   // // Get or set one of the selection rectangles
   // VIDIOC_G_SELECTION
   // VIDIOC_S_SELECTION

   // // Get or set streaming parameters
   // // VIDIOC_G_PARM - no
   // VIDIOC_S_PARM

   // // Execute an decoder command
   // VIDIOC_DECODER_CMD
   // VIDIOC_TRY_DECODER_CMD

   // // Execute an encoder command
   // VIDIOC_ENCODER_CMD
   // VIDIOC_TRY_ENCODER_CMD

   // // Enumerate image formats
   // VIDIOC_ENUM_FMT

   // // Enumerate frame sizes
   // VIDIOC_ENUM_FRAMESIZES

   // // Get or set the value of several controls, try control values
   // // VIDIOC_G_EXT_CTRLS - no
   // VIDIOC_S_EXT_CTRLS
   // // VIDIOC_TRY_EXT_CTRLS - no

   // // Subscribe or unsubscribe event
   // VIDIOC_SUBSCRIBE_EVENT
   // // VIDIOC_UNSUBSCRIBE_EVENT - no

   // // Export a buffer as a DMABUF file descriptor
   // VIDIOC_EXPBUF

   // // Dequeue event
   // VIDIOC_DQEVENT

   // // Enumerate controls and menu control items
   // VIDIOC_QUERYCTRL
   // // VIDIOC_QUERY_EXT_CTRL - no
   // VIDIOC_QUERYMENU

   // // Exchange a buffer with the driver
   // VIDIOC_QBUF
   // VIDIOC_DQBUF

   // // Query the status of a buffer
   // VIDIOC_QUERYBUF

   // // Query device capabilities
   // VIDIOC_QUERYCAP
   .vidioc_querycap = vidioc_querycap,

   // // Initiate Memory Mapping, User Pointer I/O or DMA buffer I/O
   // VIDIOC_REQBUFS

   // // Start or stop streaming I/O
   // VIDIOC_STREAMON
   // VIDIOC_STREAMOFF
};




// static const struct v4l2_file_operations vivid_fops = {
//    .owner                        = THIS_MODULE,
//    .unlocked_ioctl               = video_ioctl2,
//    .open                         = vivid_open,
//    .release                      = vivid_release,
//    .poll                         = vb2_fop_poll,
//    .mmap                         = vb2_fop_mmap,
//    .read                         = vb2_fop_read,
// };
