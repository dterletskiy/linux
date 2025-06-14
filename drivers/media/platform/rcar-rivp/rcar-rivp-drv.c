#include <linux/clk-provider.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_graph.h>
#include <linux/of_reserved_mem.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/sys_soc.h>
#include <linux/rpmsg.h>

#include "rcar-rivp.h"



static int process_dt_node( struct device* dev )
{
   struct device_node* rivp_node;
   int ret = 0;

   rivp_node = of_find_node_by_path( DT_RCAR_RIVP_MEM_NODE );
   if( NULL == rivp_node )
   {
      dev_err( dev, "of_find_node_by_path( %s ): error\n", DT_RCAR_RIVP_MEM_NODE );
      ret = -ENOMEM;
      goto error;
   }

   ret = of_reserved_mem_device_init_by_idx( dev, rivp_node, 0 );
   if( ret )
   {
      dev_err( dev, "of_reserved_mem_device_init_by_idx( %s, 0 ): %d\n",
            DT_RCAR_RIVP_MEM_NODE, ret
         );
      goto error;
   }

   return 0;
error:
   return ret;
}



static int rcar_rivp_cb(
      struct rpmsg_device* rpmsg_dev, void* data, int len, void* priv, u32 src
   )
{
   struct rivp_device* rivp_dev = dev_get_drvdata( &rpmsg_dev->dev );
   struct rivp_msg* rivp_msg = (struct rivp_msg*)data;

   switch( rivp_msg->cmd )
   {
      case CMD_FRAME_DONE:
      {
         rivp_inf( rivp_dev, "Cortex-R CMD_FRAME_DONE" );
         break;
      }

      case CMD_ERROR:
      {
         rivp_err( rivp_dev, "Cortex-R CMD_ERROR" );
         break;
      }
      default:
      {
         rivp_wrn( rivp_dev, "Undefined command" );
         break;
      }
   }

   return 0;
}

static void rcar_rivp_remove( struct rpmsg_device* rpmsg_dev )
{
   struct rivp_device* rivp_dev = dev_get_drvdata( &rpmsg_dev->dev );

   return;
}

static int rcar_rivp_probe( struct rpmsg_device* rpmsg_dev )
{
   struct rivp_device* rivp_dev;
   struct rivp_v4l2_device* rivp_v4l2_dev;
   int ret = 0;
   int i = 0;

   pr_info( "%s():%d\n", __FUNCTION__, __LINE__ );

   rivp_dev = devm_kzalloc( &rpmsg_dev->dev, sizeof(struct rivp_device), GFP_KERNEL );
   if( NULL == rivp_dev )
      return -ENOMEM;

   dev_set_drvdata( &rpmsg_dev->dev, rivp_dev );

   /* Save a link to struct device and struct rpmsg_device */
   rivp_dev->dev = &rpmsg_dev->dev;
   rivp_dev->rpmsg_dev = rpmsg_dev;

   rivp_dev->dev->coherent_dma_mask = DMA_BIT_MASK(32);

   ret = process_dt_node( rivp_dev->dev );
   if( ret )
   {
      goto error;
   }

   for( i = 0; i < rivp_dev->channel_num; ++i )
   {
      rivp_v4l2_dev = devm_kzalloc( rivp_dev->dev, sizeof(struct rivp_v4l2_device), GFP_KERNEL );
      if( NULL == rivp_v4l2_dev )
      {
         ret = -ENOMEM;
         goto error;
      }
      rivp_v4l2_dev->dev = rivp_dev->dev;
      rivp_v4l2_dev->rivp_dev = rivp_dev;
      rivp_v4l2_dev->channel = i;

      rivp_dev->rivp_v4l2_dev[i] = rivp_v4l2_dev;
   }

   return 0;
error:
   return ret;
}



static struct rpmsg_device_id taurus_driver_rivp_id_table[ ] = {
   { .name = "taurus-rivp" },
   { .name = "taurus-codec" },
   {},
};
MODULE_DEVICE_TABLE( rpmsg, taurus_driver_rivp_id_table );



static struct rpmsg_driver rivp_codec_client = {
   .drv.name   = KBUILD_MODNAME,
   .id_table   = taurus_driver_rivp_id_table,
   .probe      = rcar_rivp_probe,
   .callback   = rcar_rivp_cb,
   .remove     = rcar_rivp_remove,
};
module_rpmsg_driver( rivp_codec_client );



MODULE_DESCRIPTION( "Renesas Codec Driver" );
MODULE_LICENSE( "Dual MIT/GPL" );
