/***************************************************************************
 *                      COPYRIGHT NOTICE
 *             Copyright(C) 2024, D-Robotics Co., Ltd.
 *                     All rights reserved.
 ***************************************************************************/

#ifndef X5_ISP_CFG_H_
#define X5_ISP_CFG_H_
#include "hbn_api.h"
#include "cam_def.h"

typedef enum {
	HB_FALSE = 0,
	HB_TRUE = 1,
} HB_BOOL;

typedef struct image_rect_s {
	int32_t x;
	int32_t y;
	uint32_t width;
	uint32_t height;
} image_rect_t;

typedef struct crop_cfg_s {
	HB_BOOL enable;
	image_rect_t rect;
} crop_cfg_t;

typedef struct isp_sw_ctrl_s {
	uint32_t ae_stat_buf_en;
	uint32_t awb_stat_buf_en;
	uint32_t ae5bin_stat_buf_en;
	uint32_t ctx_buf_en;
	uint32_t pixel_consistency_en;
} isp_sw_ctrl_t;

typedef enum enum_isp_channel_mode_e {
	ISP_CHANNEL_MODE_NORMAL = 0,
	ISP_CHANNEL_MODE_MASTER = 1,
	ISP_CHANNEL_MODE_SLAVE = 2,

	ISP_CHANNEL_MODE_BUTT
} isp_channel_mode_e;

typedef struct isp_bind_channel_s {
	uint32_t bind_hw_id;
	uint32_t bind_slot_id;
} isp_bind_channel_t;

typedef struct isp_combine_s {
	isp_channel_mode_e isp_channel_mode; /* RW: 0: normal no need combine the different isp channels
                                                    1: master channel when needing combine isp slave channel
                                                    2: slave channel when needing combine isp master channel*/
	isp_bind_channel_t bind_channel; /* RW: if isp_channel_mode = 0, the bind_channel invaild, no need care
                                           if isp_channel_mode = master, the bind_channel is that the slave channel
                                           if isp_channel_mode = slave, the bind_channel is that the master channel*/
} isp_combine_t;

typedef enum enum_sched_mode_e {
	SCHED_MODE_TDMF = 0,
	SCHED_MODE_MANUAL = 1,
	SCHED_MODE_PASS_THRU = 2,

	SCHED_MODE_BUTT
} sched_mode_e;

typedef struct isp_channel_s {
	uint32_t hw_id;
	uint32_t slot_id;
	int32_t ctx_id; /* defined by vpf*/
} isp_channel_t;

typedef enum enum_isp_work_mode_e {
	ISP_WORK_MODE_NOMAL = 0,
	ISP_WORK_MODE_TPG = 1,
	ISP_WORK_MODE_CIM_TPG = 2,

	ISP_WORK_MODE_BUTT
} isp_work_mode_e;

typedef enum enum_hdr_mode_e {
	HDR_MODE_LINEAR = 0, /* linear mode */
	HDR_MODE_NATIVE, /* hdr mode composited by sensor */

	HDR_MODE_2To1_LINE,
	HDR_MODE_2To1_FRAME,

	HDR_MODE_3To1_LINE,
	HDR_MODE_3To1_FRAME,

	HDR_MODE_4To1_LINE,
	HDR_MODE_4To1_FRAME,

	HDR_MODE_BUTT,
} hdr_mode_e;

typedef struct isp_attr_s {
	isp_channel_t channel;
	sched_mode_e sched_mode;
	isp_work_mode_e work_mode; /* RW: isp work mode*/
	hdr_mode_e hdr_mode; /* RW; HDR mode select,include hdr mode or linear mode*/
	image_size_t size; /* RW; Range: width:(0, 4096].height(0, 2560];
                                                    Width and height of the image input from the sensor*/
	uint32_t frame_rate; /* RW; Range: (0, 120] U32.0; For frame rate */
	isp_combine_t isp_combine; /* RW; image adjustment method fusion for one camera src+roi or
                                                    double cameras in different isp channels*/
	isp_sw_ctrl_t isp_sw_ctrl;
	uint32_t algo_state;
	uint32_t clear_record;
} isp_attr_t;

typedef struct isp_ichn_attr_s {
	crop_cfg_t input_crop_cfg; /* RW; Range: crop window <= size;
                                                    Only support input0 ~ input3, Start position of the cropping window,
                                                    image width, and image height */

	uint32_t in_buf_noclean;
	uint32_t in_buf_noncached;
} isp_ichn_attr_t;

typedef enum tag_isp_stream_output_mode_e {
	STREAM_OUTPUT_MODE_DISABLE = 0, /* disable stream output */
	STREAM_OUTPUT_MODE_ENABLE = 1, /* enable stream output */
	STREAM_OUTPUT_MODE_BUTT
} isp_stream_output_mode_e;

typedef enum tag_isp_axi_output_mode_e {
	AXI_OUTPUT_MODE_DISABLE = 0,
	AXI_OUTPUT_MODE_RGB888 = 1,
	AXI_OUTPUT_MODE_RAW8 = 2,
	AXI_OUTPUT_MODE_RAW10 = 3,
	AXI_OUTPUT_MODE_RAW12 = 4,
	AXI_OUTPUT_MODE_RAW16 = 5,
	AXI_OUTPUT_MODE_RAW24 = 6,
	AXI_OUTPUT_MODE_YUV444 = 7,
	AXI_OUTPUT_MODE_YUV422 = 8, /* yuv422 */
	AXI_OUTPUT_MODE_YUV420 = 9, /* yuv420 */
	AXI_OUTPUT_MODE_IR8 = 10,
	AXI_OUTPUT_MODE_YUV420_RAW12 = 11, /* yuv420 & raw12 */
	AXI_OUTPUT_MODE_YUV422_RAW12 = 12, /* yuv422 & raw12 */
	AXI_OUTPUT_MODE_YUV420_RAW16 = 13, /* yuv420 & raw16 */
	AXI_OUTPUT_MODE_YUV422_RAW16 = 14, /* yuv422 & raw16 */

	AXI_OUTPUT_MODE_BUTT
} isp_axi_output_mode_e;

typedef enum enum_isp_output_mode_e {
	ISP_OUTPUT_RAW_LEVEL_SENSOR_DATA = 0, /* the raw data after companded sensor data, it means the unprocessed raw
	                                            data from sensor */
	ISP_OUTPUT_RAW_LEVEL_FRAME_STITCH = 1, /* the raw data after frame stitch */
	ISP_OUTPUT_RAW_LEVEL_GAMMA_FE = 2, /* the raw data after post gamma fe companding curve*/
	ISP_OUTPUT_RAW_LEVEL_TONE_MAPPED = 3, /* the raw data after tone mapped*/
	ISP_OUTPUT_RAW_LEVEL_POST_GAMMA_FE_LUT = 4, /* the raw data after post gamma fe lockup table*/
	ISP_OUTPUT_RAW_LEVEL_POST_SHADING = 5, /* the raw data after post shading*/

	ISP_OUTPUT_RAW_LEVEL_BUTT
} isp_output_raw_level_e;

typedef struct isp_ochn_attr_s {
	isp_stream_output_mode_e stream_output_mode; /* RW, STREAM_OUTPUT_MODE_DISABLE can be used to disable.*/
	isp_axi_output_mode_e axi_output_mode; /* RW, AXI_OUTPUT_MODE_DISABLE can be used to disable.*/

	crop_cfg_t output_crop_cfg; /* RW; Range: crop window <= input_crop_cfg.rect;
                                                    position of the cropping window, image width, and image height */

	isp_output_raw_level_e output_raw_level;
	uint32_t out_buf_noinvalid;
	uint32_t out_buf_noncached;
	uint32_t buf_num;
} isp_ochn_attr_t;


enum isp_ochn_channel_type_e {
	ISP_MAIN_FRAME,
	ISP_CHN_MAX,
};

typedef enum isp_sensor_mode_e {
	ISP_NORMAL_M = 0,
	ISP_DOL2_M = 1,
	ISP_PWL_M = 2,
	ISP_INVALID_MOD,
} isp_sensor_mode_t;

//typedef enum enum_mcm_sched_mode_e {
//	ROUND_ROBIN = 0,
//	FIXED_SEQUENCE,
//	FIFO,
//} sched_mode_e;

typedef enum enum_input_mode_e {
	PASSTHROUGH_MODE = 0,
	MCM_MODE,
	DDR_MODE,
} input_mode_e;

//typedef struct isp_attr_s {
//	uint32_t input_mode;
//	uint32_t sched_mode;
//	uint32_t tile_mode;
//	isp_sensor_mode_t sensor_mode;
//	common_rect_t crop;
//} isp_attr_t;

typedef struct isp_cfg_s {
	isp_attr_t isp_attr;
	isp_ichn_attr_t ichn_attr;
	isp_ochn_attr_t ochn_attr;
} isp_cfg_t;

int32_t isp_node_parser_config(const void *root, isp_cfg_t *cfg);
#endif // X5_ISP_CFG_H_
