/**
Copyright 2014-2017 Robert McNeel and Associates

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**/

#ifndef __CYCLES__H__
#define __CYCLES__H__

#ifdef WIN32
#ifdef CCL_CAPI_DLL
#define CCL_CAPI __declspec (dllexport)
#else
#define CCL_CAPI __declspec (dllimport)
#endif
#else
#define CCL_CAPI
#endif
/*

// conversion matrix for rhino -> cycles view.
ccl::Transform camConvertMat = ccl::make_transform(
1.0f, 0.0f, 0.0f, 0.0f,
0.0f, -1.0f, 0.0f, 0.0f,
0.0f, 0.0f, -1.0f, 1.0f

*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup ccycles CCycles
 * CCycles is the low-level C-API for Cycles. Using this API one can
 * set up the Cycles render engine, push geometry and shaders to it
 * and govern the render process.
 */
/**
 * \defgroup ccycles_scene Scene API
 * The CCycles scene API provides functions to set up a scene for Cycles.
 * A scene will hold object, mesh, light and shader information for the
 * render engine to use during the render process.
 * \ingroup ccycles
 */
/**
 * \defgroup ccycles_shader Shader API
 * The CCycles shader API provides functions to create Cycles
 * shaders giving access to all available shader nodes.
 * \ingroup ccycles
 */
/**
 * \defgroup ccycles_mesh Mesh API
 * The CCycles mesh API provides functions to create Cycles
 * meshes.
 * \ingroup ccycles
 */
/**
 * \defgroup ccycles_object Object API
 * The CCycles object API provides functions to create Cycles
 * objects.
 * \ingroup ccycles
 */
/**
 * \defgroup ccycles_session Session API
 * The CCycles session API provides functions to create Cycles
 * sessions, administer callbacks and manage render processes.
 * \ingroup ccycles
 */

/***********************************/

/**
 * Logger function signature. Used to register a logging
 * function with CCycles using
 * \ingroup ccycles
 */
typedef void(__cdecl *LOGGER_FUNC_CB)(const char* msg);

/**
 * Status update function signature. Used to register a status
 * update callback function with CCycles using cycles_session_set_update_callback
 * \ingroup ccycles ccycles_session
 */
typedef void(__cdecl *STATUS_UPDATE_CB)(unsigned int session_id);

/**
 * Test cancel function signature. Used to test for cancel condition
 * test callback function with CCycles using cycles_session_set_cancel_callback
 * \ingroup ccycles ccycles_session
 */
typedef void(__cdecl *TEST_CANCEL_CB)(unsigned int session_id);

/**
 * Render tile update or write function signature. Used to register a render tile
 * update callback function with CCycles using cycles_session_set_write_tile_callback or
 * cycles_session_set_update_tile_callback
 * \ingroup ccycles ccycles_session
 */
typedef void(__cdecl *RENDER_TILE_CB)(unsigned int session_id, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int sample, unsigned int depth, int passtype, float* pixels, int pixlen);

/**
 * Pixel buffer from DisplayBuffer update function signature.
 * Set update function to CCycles using cycles_session_set_display_update_callback
 * \ingroup ccycles ccycles_session
 */
typedef void(__cdecl *DISPLAY_UPDATE_CB)(unsigned int session_id, unsigned int sample);


/**
 * Initialise Cycles by querying available devices.
 * \ingroup ccycles
 */
CCL_CAPI void __cdecl cycles_initialise(unsigned int mask = ccl::DEVICE_MASK_ALL);

/**
 * Initialise paths for Cycles to look in (pre-compiled kernels, cached kernels, kernel code)
 * \ingroup ccycles
 */
CCL_CAPI void __cdecl cycles_path_init(const char* path, const char* user_path);

/**
 * Set an environment variable for Cycles
 * \ingroup ccycles
 */
CCL_CAPI void __cdecl cycles_putenv(const char* var, const char* val);

/**
 * Set the CPU kernel type to use. 1 is split, 0 is regular.
 * \ingroup ccycles
 */
CCL_CAPI void __cdecl cycles_debug_set_cpu_kernel(unsigned int state);

/**
 * Pass 1 to allow QBVH usage in CPU kernel.
 * \ingroup ccycles
 */
CCL_CAPI void __cdecl cycles_debug_set_cpu_allow_qbvh(unsigned int state);

/**
 * Set the CUDA kernel type to use. 1 is split, 0 is regular.
 * \ingroup ccycles
 */
CCL_CAPI void __cdecl cycles_debug_set_cuda_kernel(unsigned int state);

/**
 * Set the OpenCL kernel type to use. 1 is split, 0 is mega. -1 means decide
 * automatically based on officially supported devices.
 * \ingroup ccycles
 */
CCL_CAPI void __cdecl cycles_debug_set_opencl_kernel(int state);

/**
 * Set the OpenCL kernel to be compiled as single program with 1.
 * \ingroup ccycles
 */
CCL_CAPI void __cdecl cycles_debug_set_opencl_single_program(int state);

/**
 * Set the OpenCL device type allowed.
 * 0 = none
 * 1 = all
 * 2 = default
 * 3 = CPU
 * 4 = GPU
 * 5 = accelerator
 */
CCL_CAPI void __cdecl cycles_debug_set_opencl_device_type(int type);

/**
 * Clean up everything, we're done.
 * \ingroup ccycles
 * \todo Add session specific cleanup, so we don't accidently delete sessions that are in progress.
 */
CCL_CAPI void __cdecl cycles_shutdown();

/**
 * Add a logger function. This will be called only in debug builds.
 * \ingroup ccycles
 */
CCL_CAPI void __cdecl cycles_set_logger(unsigned int client_id, LOGGER_FUNC_CB logger_func_);

/**
 * Set to true if logger output should be sent to std::cout as well.
 *
 * Note that this is global to the logger.
 */
CCL_CAPI void __cdecl cycles_log_to_stdout(int tostdout);

/**
 * Create a new client.
 *
 * This is mainly used for determining what logger functions belong to which client and session.
 *
 * Note that a client needs to be released with cycles_release_client when no longer necessary.
 */
CCL_CAPI unsigned int __cdecl cycles_new_client();

/**
 * Release a client from usage.
 */
CCL_CAPI void __cdecl cycles_release_client(unsigned int client_id);

/**
 * Query number of available devices.
 * \ingroup ccycles
 */
CCL_CAPI unsigned int __cdecl cycles_number_devices();

/**
 * Query number of available (created) multi-devices.
 * \ingroup ccycles
 */
CCL_CAPI unsigned int __cdecl cycles_number_multidevices();

/**
 * Query number of devices in multi-device
 * \ingroup ccycles
 */
CCL_CAPI unsigned int __cdecl cycles_number_multi_subdevices(int i);

/**
 * Query the index of the sub-device in the global device list.
 * \ingroup ccycles
 */
CCL_CAPI unsigned int __cdecl cycles_get_multidevice_subdevice_id(int i, int j);

/* Query number of available CUDA devices. */
CCL_CAPI unsigned int __cdecl cycles_number_cuda_devices();

/* Query name of a device. */
CCL_CAPI const char* __cdecl cycles_device_description(int i);

/* Query capabilities of all devices found. */
CCL_CAPI const char* __cdecl cycles_device_capabilities();

/* Query ID of a device. */
CCL_CAPI const char* __cdecl cycles_device_id(int i);

/* Query the index of a device. The index is the nth for the type the device is of. */
CCL_CAPI int __cdecl cycles_device_num(int i);

/* Query if device supports advanced shading. */
CCL_CAPI bool __cdecl cycles_device_advanced_shading(int i);

/* Query if device is used as display device. */
CCL_CAPI bool __cdecl cycles_device_display_device(int i);

/* Create or get multi device. Return value is index of multi-device in multi-device vector.*/
CCL_CAPI int __cdecl cycles_create_multidevice(int count, int* idx);

/** Query device type.
 * \param i device ID.
 * \returns device type
 * \retval 0 None
 * \retval 1 CPU
 * \retval 2 OPENCL
 * \retval 3 CUDA
 * \retval 4 NETWORK
 * \retval 5 MULTI
 */
CCL_CAPI unsigned int __cdecl cycles_device_type(int i);

/* Create scene parameters, to be used when creating a new scene. */
CCL_CAPI unsigned int __cdecl cycles_scene_params_create(unsigned int client_id, unsigned int shadingsystem, unsigned int bvh_type, unsigned int use_bvh_spatial_split, int bvh_layout, unsigned int persistent_data);

/* Set scene parameters*/

/** Set scene parameter: BVH type. */
CCL_CAPI void __cdecl cycles_scene_params_set_bvh_type(unsigned int client_id, unsigned int scene_params_id, unsigned int type);
/** Set scene parameter: use BVH spatial split. */
CCL_CAPI void __cdecl cycles_scene_params_set_bvh_spatial_split(unsigned int client_id, unsigned int scene_params_id, unsigned int use);
/** Set scene parameter: use qBVH. */
CCL_CAPI void __cdecl cycles_scene_params_set_qbvh(unsigned int client_id, unsigned int scene_params_id, unsigned int use);
/** Set scene parameter: Shading system (SVM / OSL).
 * Note that currently SVM is only supported for RhinoCycles. No effort yet has been taken to enable OSL.
 */
CCL_CAPI void __cdecl cycles_scene_params_set_shadingsystem(unsigned int client_id, unsigned int scene_params_id, unsigned int system);
/** Set scene parameter: use persistent data. */
CCL_CAPI void __cdecl cycles_scene_params_set_persistent_data(unsigned int client_id, unsigned int scene_params_id, unsigned int use);

/**
 * Create a new mesh in scene_id, using shader_id
 * \ingroup ccycles_scene
 */
CCL_CAPI unsigned int __cdecl cycles_scene_add_mesh(unsigned int client_id, unsigned int scene_id, unsigned int shader_id);
/**
 * Create a new mesh for object_id in scene_id, using shader_id
 * \ingroup ccycles_scene
 */
CCL_CAPI unsigned int __cdecl cycles_scene_add_mesh_object(unsigned int client_id, unsigned int scene_id, unsigned int object_id, unsigned int shader_id);
/**
 * Create a new object for scene_id
 * \ingroup ccycles_scene
 */
CCL_CAPI unsigned int __cdecl cycles_scene_add_object(unsigned int client_id, unsigned int scene_id);
/**
 * Set transformation matrix for object
 * \ingroup ccycles_object
 */
CCL_CAPI void __cdecl cycles_scene_object_set_matrix(unsigned int client_id, unsigned int scene_id, unsigned int object_id,
	float a, float b, float c, float d,
	float e, float f, float g, float h,
	float i, float j, float k, float l
	);
/**
 * Set OCS frame for object
 * \ingroup ccycles_object
 */
CCL_CAPI void __cdecl cycles_scene_object_set_ocs_frame(unsigned int client_id, unsigned int scene_id, unsigned int object_id,
	float a, float b, float c, float d,
	float e, float f, float g, float h,
	float i, float j, float k, float l
	);
/**
 * Set object mesh
 * \ingroup ccycles_object
 */
CCL_CAPI void __cdecl cycles_scene_object_set_mesh(unsigned int client_id, unsigned int scene_id, unsigned int object_id, unsigned int mesh_id);
/**
 * Get mesh id for object
 * \ingroup ccycles_object
 */
CCL_CAPI unsigned int __cdecl cycles_scene_object_get_mesh(unsigned int client_id, unsigned int scene_id, unsigned int object_id);
/**
 * Set visibility flag for object
 * \ingroup ccycles_object
 */
CCL_CAPI void __cdecl cycles_scene_object_set_visibility(unsigned int client, unsigned int scene_id, unsigned int object_id, unsigned int visibility);
/**
 * Set object shader
 * \ingroup ccycles_object
 */
CCL_CAPI void __cdecl cycles_scene_object_set_shader(unsigned int client, unsigned int scene_id, unsigned int object_id, unsigned int shader_id);
/**
 * Set is_shadow_catcher flag for object
 * \ingroup ccycles_object
 */
CCL_CAPI void __cdecl cycles_scene_object_set_is_shadowcatcher(unsigned int client, unsigned int scene_id, unsigned int object_id, bool is_shadowcatcher);
/**
 * Set mesh_light_no_cast_shadow flag for object. This is to signal that this mesh light shouldn't cast shadows.
 * \ingroup ccycles_object
 */
CCL_CAPI void __cdecl cycles_scene_object_set_mesh_light_no_cast_shadow(unsigned int client, unsigned int scene_id, unsigned int object_id, bool mesh_light_no_cast_shadow);
/**
 * Set is_block_instance flag for object. This ensures we can handle meshes
 * properly also when only one block instance for a mesh is in the scene.
 * \ingroup ccycles_object
 */
CCL_CAPI void __cdecl cycles_scene_object_set_is_block_instance(unsigned int client, unsigned int scene_id, unsigned int object_id, bool is_block_instance);
/**
 * Set cutout flag for object. This object is used for cutout/clipping.
 * \ingroup ccycles_object
 */
CCL_CAPI void __cdecl cycles_scene_object_set_cutout(unsigned int client, unsigned int scene_id, unsigned int object_id, bool cutout);
/**
 * Set ignore_cutout flag for object. Ignore cutout object.
 * \ingroup ccycles_object
 */
CCL_CAPI void __cdecl cycles_scene_object_set_ignore_cutout(unsigned int client, unsigned int scene_id, unsigned int object_id, bool ignore_cutout);
/**
 * Tag object for update
 * \ingroup ccycles_object
 */
CCL_CAPI void __cdecl cycles_object_tag_update(unsigned int client_id, unsigned int scene_id, unsigned int object_id);

/**
 * Set the pass id
 * \ingroup ccycles_object
 */
CCL_CAPI void __cdecl cycles_object_set_pass_id(unsigned int client_id, unsigned int scene_id, unsigned int object_id, int pass_id);

/**
 * Set the random id
 * \ingroup ccycles_object
 */
CCL_CAPI void __cdecl cycles_object_set_random_id(unsigned int client_id, unsigned int scene_id, unsigned int object_id, unsigned int random_id);

/**
 * Clear clipping planes list.
 */
CCL_CAPI void __cdecl cycles_scene_clear_clipping_planes(unsigned int client_id, unsigned int scene_id);

/**
 * Add a clipping plane equation.
 */
CCL_CAPI unsigned int __cdecl cycles_scene_add_clipping_plane(unsigned int client_id, unsigned int scene_id, float a, float b, float c, float d);

/**
 * Discard clipping plane (abcd are all set to FLT_MAX).
 */
CCL_CAPI void __cdecl cycles_scene_discard_clipping_plane(unsigned int client_id, unsigned int scene_id, unsigned int cp_id);

/**
 * Set a clipping plane equation.
 */
CCL_CAPI void __cdecl cycles_scene_set_clipping_plane(unsigned int client_id, unsigned int scene_id, unsigned int cp_id, float a, float b, float c, float d);
/** Tag integrator for update. */
CCL_CAPI void __cdecl cycles_integrator_tag_update(unsigned int client_id, unsigned int scene_id);
/** Set the maximum bounces for integrator. */
CCL_CAPI void __cdecl cycles_integrator_set_max_bounce(unsigned int client_id, unsigned int scene_id, int max_bounce);
/** Set the minimum bounces for integrator. */
CCL_CAPI void __cdecl cycles_integrator_set_min_bounce(unsigned int client_id, unsigned int scene_id, int min_bounce);
/** Set to true if caustics should be skipped.
 * \todo split for caustics_reflective and caustics_refractive.
 */
CCL_CAPI void __cdecl cycles_integrator_set_no_caustics(unsigned int client_id, unsigned int scene_id, bool no_caustics);
/** Set to true if shadows shouldn't be traced.
 */
CCL_CAPI void __cdecl cycles_integrator_set_no_shadows(unsigned int client_id, unsigned int scene_id, bool no_shadows);
/** Set the amount of diffuse samples. */
CCL_CAPI void __cdecl cycles_integrator_set_diffuse_samples(unsigned int client_id, unsigned int scene_id, int diffuse_samples);
/** Set the amount of glossy samples. */
CCL_CAPI void __cdecl cycles_integrator_set_glossy_samples(unsigned int client_id, unsigned int scene_id, int glossy_samples);
/** Set the amount of transmission samples. */
CCL_CAPI void __cdecl cycles_integrator_set_transmission_samples(unsigned int client_id, unsigned int scene_id, int transmission_samples);
/** Set the amount of AO samples. */
CCL_CAPI void __cdecl cycles_integrator_set_ao_samples(unsigned int client_id, unsigned int scene_id, int ao_samples);
/** Set the amount of mesh light samples. */
CCL_CAPI void __cdecl cycles_integrator_set_mesh_light_samples(unsigned int client_id, unsigned int scene_id, int mesh_light_samples);
/** Set the amount of SSS samples. */
CCL_CAPI void __cdecl cycles_integrator_set_subsurface_samples(unsigned int client_id, unsigned int scene_id, int subsurface_samples);
/** Set the amount of volume samples. */
CCL_CAPI void __cdecl cycles_integrator_set_volume_samples(unsigned int client_id, unsigned int scene_id, int volume_samples);
/** Set the maximum amount of diffuse bounces. */
CCL_CAPI void __cdecl cycles_integrator_set_max_diffuse_bounce(unsigned int client_id, unsigned int scene_id, int max_diffuse_bounce);
/** Set the maximum amount of glossy bounces. */
CCL_CAPI void __cdecl cycles_integrator_set_max_glossy_bounce(unsigned int client_id, unsigned int scene_id, int max_glossy_bounce);
/** Set the maximum amount of transmission bounces. */
CCL_CAPI void __cdecl cycles_integrator_set_max_transmission_bounce(unsigned int client_id, unsigned int scene_id, int max_transmission_bounce);
/** Set the maximum amount of volume bounces. */
CCL_CAPI void __cdecl cycles_integrator_set_max_volume_bounce(unsigned int client_id, unsigned int scene_id, int max_volume_bounce);
/** Set the maximum amount of transparency bounces. */
CCL_CAPI void __cdecl cycles_integrator_set_transparent_max_bounce(unsigned int client_id, unsigned int scene_id, int transparent_max_bounce);
/** Set the minimum amount of transparency bounces. */
CCL_CAPI void __cdecl cycles_integrator_set_transparent_min_bounce(unsigned int client_id, unsigned int scene_id, int transparent_min_bounce);
/** Set the amount of AA samples. */
CCL_CAPI void __cdecl cycles_integrator_set_aa_samples(unsigned int client_id, unsigned int scene_id, int aa_samples);
/** Set the glossiness filter. */
CCL_CAPI void __cdecl cycles_integrator_set_filter_glossy(unsigned int client_id, unsigned int scene_id, float filter_glossy);
/** Set integrator method to use (path, branched path).*/
CCL_CAPI void __cdecl cycles_integrator_set_method(unsigned int client_id, unsigned int scene_id, int method);
/** Set to true if all lights should be directly sampled. */
CCL_CAPI void __cdecl cycles_integrator_set_sample_all_lights_direct(unsigned int client_id, unsigned int scene_id, bool sample_all_lights_direct);
/** Set to true if all lights should be indirectly sampled. */
CCL_CAPI void __cdecl cycles_integrator_set_sample_all_lights_indirect(unsigned int client_id, unsigned int scene_id, bool sample_all_lights_indirect);
CCL_CAPI void __cdecl cycles_integrator_set_volume_step_size(unsigned int client_id, unsigned int scene_id, float volume_step_size);
CCL_CAPI void __cdecl cycles_integrator_set_volume_max_steps(unsigned int client_id, unsigned int scene_id, int volume_max_steps);
/* \todo update Cycles code to allow for caustics form separation
void cycles_integrator_set_caustics_relective(unsigned int client_id, unsigned int scene_id, int caustics_relective)
void cycles_integrator_set_caustics_refractive(unsigned int client_id, unsigned int scene_id, int caustics_refractive)
*/
CCL_CAPI void __cdecl cycles_integrator_set_seed(unsigned int client_id, unsigned int scene_id, int seed);

enum class sampling_pattern : unsigned int {
	SOBOL = 0,
	CMJ
};
CCL_CAPI void __cdecl cycles_integrator_set_sampling_pattern(unsigned int client_id, unsigned int scene_id, sampling_pattern pattern);
CCL_CAPI void __cdecl cycles_integrator_set_sample_clamp_direct(unsigned int client_id, unsigned int scene_id, float sample_clamp_direct);
CCL_CAPI void __cdecl cycles_integrator_set_sample_clamp_indirect(unsigned int client_id, unsigned int scene_id, float sample_clamp_indirect);
CCL_CAPI void __cdecl cycles_integrator_set_light_sampling_threshold(unsigned int client_id, unsigned int scene_id, float light_sampling_threshold);

/** Different camera types. */
enum class camera_type : unsigned int {
	PERSPECTIVE = 0,
	ORTHOGRAPHIC,
	PANORAMA
};

/** Different panorama types of camera. */
enum class panorama_type : unsigned int {
	EQUIRECTANGLUAR = 0,
	FISHEYE_EQUIDISTANT,
	FISHEYE_EQUISOLID
};

/** Set the size/resolution of the camera. This equals to pixel resolution. */
CCL_CAPI void __cdecl cycles_camera_set_size(unsigned int client_id, unsigned int scene_id, unsigned int width, unsigned int height);
/** Get the camera width. */
CCL_CAPI unsigned int __cdecl cycles_camera_get_width(unsigned int client_id, unsigned int scene_id);
/** Get the camera height. */
CCL_CAPI unsigned int __cdecl cycles_camera_get_height(unsigned int client_id, unsigned int scene_id);
/** Set the camera type. */
CCL_CAPI void __cdecl cycles_camera_set_type(unsigned int client_id, unsigned int scene_id, camera_type type);
/** Set the camera panorama type. */
CCL_CAPI void __cdecl cycles_camera_set_panorama_type(unsigned int client_id, unsigned int scene_id, panorama_type type);
/** Set the transformation matrix for the camera. */
CCL_CAPI void __cdecl cycles_camera_set_matrix(unsigned int client_id, unsigned int scene_id,
	float a, float b, float c, float d,
	float e, float f, float g, float h,
	float i, float j, float k, float l
	);
/** Compute the auto viewplane for scene camera. */
CCL_CAPI void __cdecl cycles_camera_compute_auto_viewplane(unsigned int client_id, unsigned int scene_id);
/** Set viewplane for scene camera. */
CCL_CAPI void __cdecl cycles_camera_set_viewplane(unsigned int client_id, unsigned int scene_id, float left, float right, float top, float bottom);
/** Update camera. Should be called after changing settings on a scene camera. */
CCL_CAPI void __cdecl cycles_camera_update(unsigned int client_id, unsigned int scene_id);
/** Set the Field of View for scene camera. */
CCL_CAPI void __cdecl cycles_camera_set_fov(unsigned int client_id, unsigned int scene_id, float fov);
/** Set the sensor width for scene camera. */
CCL_CAPI void __cdecl cycles_camera_set_sensor_width(unsigned int client_id, unsigned int scene_id, float sensor_width);
/** Set the sensor height for scene camera. */
CCL_CAPI void __cdecl cycles_camera_set_sensor_height(unsigned int client_id, unsigned int scene_id, float sensor_height);
/** Set the near clip for scene camera. */
CCL_CAPI void __cdecl cycles_camera_set_nearclip(unsigned int client_id, unsigned int scene_id, float nearclip);
/** Set the far clip for scene camera. */
CCL_CAPI void __cdecl cycles_camera_set_farclip(unsigned int client_id, unsigned int scene_id, float farclip);
/** Set the aperture size for scene camera. */
CCL_CAPI void __cdecl cycles_camera_set_aperturesize(unsigned int client_id, unsigned int scene_id, float aperturesize);
/** Set the aperture ratio for anamorphic lens bokeh. */
CCL_CAPI void __cdecl cycles_camera_set_aperture_ratio(unsigned int client_id, unsigned int scene_id, float aperture_ratio);
/** Set camera blades count. */
CCL_CAPI void __cdecl cycles_camera_set_blades(unsigned int client_id, unsigned int scene_id, unsigned int blades);
/** Set camera blade rotation. */
CCL_CAPI void __cdecl cycles_camera_set_bladesrotation(unsigned int client_id, unsigned int scene_id, float bladesrotation);
/** Set the focal distance for scene camera. */
CCL_CAPI void __cdecl cycles_camera_set_focaldistance(unsigned int client_id, unsigned int scene_id, float focaldistance);
/** Set the shutter time for scene camera. Used mainly with motion blur aspect of rendering process. */
CCL_CAPI void __cdecl cycles_camera_set_shuttertime(unsigned int client_id, unsigned int scene_id, float shuttertime);
/** Set the field of view for fisheye camera. */
CCL_CAPI void __cdecl cycles_camera_set_fisheye_fov(unsigned int client_id, unsigned int scene_id, float fisheye_fov);
/** Set the lens for fisheye camera. */
CCL_CAPI void __cdecl cycles_camera_set_fisheye_lens(unsigned int client_id, unsigned int scene_id, float fisheye_lens);

/** Create a new session for scene id. */
CCL_CAPI unsigned int __cdecl cycles_session_create(unsigned int client_id, unsigned int session_params_id);
CCL_CAPI void __cdecl cycles_session_set_scene(unsigned int client_id, unsigned int session_id, unsigned int scene_id);

/** Reset session. */
CCL_CAPI int __cdecl cycles_session_reset(unsigned int client_id, unsigned int session_id, unsigned int width, unsigned int height, unsigned int samples, unsigned int full_x, unsigned int full_y, unsigned int full_width, unsigned int full_height );

CCL_CAPI void __cdecl cycles_session_add_pass(unsigned int client_id, unsigned int session_id, int pass_id);
CCL_CAPI void __cdecl cycles_session_clear_passes(unsigned int client_id, unsigned int session_id);

/** Set the status update callback for session. */
CCL_CAPI void __cdecl cycles_session_set_update_callback(unsigned int client_id, unsigned int session_id, void(*update)(unsigned int sid));
/** Set the test cancel callback for session. */
CCL_CAPI void __cdecl cycles_session_set_cancel_callback(unsigned int client_id, unsigned int session_id, void(*cancel)(unsigned int sid));
/** Set the render tile update callback for session. */
CCL_CAPI void __cdecl cycles_session_set_update_tile_callback(unsigned int client_id, unsigned int session_id, RENDER_TILE_CB update_tile_cb);
/** Set the render tile write callback for session. */
CCL_CAPI void __cdecl cycles_session_set_write_tile_callback(unsigned int client_id, unsigned int session_id, RENDER_TILE_CB write_tile_cb);
/** Set the display update callback for session. */
CCL_CAPI void __cdecl cycles_session_set_display_update_callback(unsigned int client_id, unsigned int session_id, DISPLAY_UPDATE_CB display_update_cb);
/** Cancel session with cancel_message for log. */
CCL_CAPI void __cdecl cycles_session_cancel(unsigned int client_id, unsigned int session_id, const char *cancel_message);
/** Start given session render process. */
CCL_CAPI void __cdecl cycles_session_start(unsigned int client_id, unsigned int session_id);
/** Wait for session render process to finish or cancel. */
CCL_CAPI void __cdecl cycles_session_wait(unsigned int client_id, unsigned int session_id);
/** Pause (true) or un-pause (false) a render session. */
CCL_CAPI void __cdecl cycles_session_set_pause(unsigned int client_id, unsigned int session_id, bool pause);
/** True if session is paused. */
CCL_CAPI bool __cdecl cycles_session_is_paused(unsigned int client_id, unsigned int session_id);
/** Set session samples to render. */
CCL_CAPI void __cdecl cycles_session_set_samples(unsigned int client_id, unsigned int session_id, int samples);
/** Clear resources for session. */
CCL_CAPI void __cdecl cycles_session_destroy(unsigned int client_id, unsigned int session_id, unsigned int scene_id);
CCL_CAPI void __cdecl cycles_session_get_float_buffer(unsigned int client_id, unsigned int session_id, int passtype, float** pixels);
/** Get pixel data buffer pointer. */
CCL_CAPI void __cdecl cycles_session_prepare_run(unsigned int client_id, unsigned int session_id);
CCL_CAPI int __cdecl cycles_session_sample(unsigned int client_id, unsigned int session_id);
CCL_CAPI void __cdecl cycles_session_end_run(unsigned int client_id, unsigned int session_id);


/* session progress access. */
CCL_CAPI void __cdecl cycles_progress_reset(unsigned int client_id, unsigned int session_id);
CCL_CAPI int __cdecl cycles_progress_get_sample(unsigned int client_id, unsigned int session_id);
CCL_CAPI void __cdecl cycles_progress_get_time(unsigned int client_id, unsigned int session_id, double* total_time, double* sample_time);
CCL_CAPI void __cdecl cycles_tilemanager_get_sample_info(unsigned int client_id, unsigned int session_id, unsigned int* samples, unsigned int* total_samples);
CCL_CAPI void __cdecl cycles_progress_get_progress(unsigned int client_id, unsigned int session_id, float* progress);
CCL_CAPI bool __cdecl cycles_progress_get_status(unsigned int client_id, unsigned int session_id, void* strholder);
CCL_CAPI bool __cdecl cycles_progress_get_substatus(unsigned int client_id, unsigned int session_id, void* strholder);
CCL_CAPI void* __cdecl cycles_string_holder_new();
CCL_CAPI const char* __cdecl cycles_string_holder_get(void* strholder);
CCL_CAPI void __cdecl cycles_string_holder_delete(void* strholder);

CCL_CAPI unsigned int __cdecl cycles_session_params_create(unsigned int client_id, unsigned int device);

CCL_CAPI void __cdecl cycles_session_params_set_device(unsigned int client_id, unsigned int session_params_id, unsigned int device);
CCL_CAPI void __cdecl cycles_session_params_set_background(unsigned int client_id, unsigned int session_params_id, unsigned int background);
CCL_CAPI void __cdecl cycles_session_params_set_progressive_refine(unsigned int client_id, unsigned int session_params_id, unsigned int progressive_refine);
CCL_CAPI void __cdecl cycles_session_params_set_output_path(unsigned int client_id, unsigned int session_params_id, const char *output_path);
CCL_CAPI void __cdecl cycles_session_params_set_progressive(unsigned int client_id, unsigned int session_params_id, unsigned int progressive);
CCL_CAPI void __cdecl cycles_session_params_set_experimental(unsigned int client_id, unsigned int session_params_id, unsigned int experimental);
CCL_CAPI void __cdecl cycles_session_params_set_samples(unsigned int client_id, unsigned int session_params_id, int samples);
CCL_CAPI void __cdecl cycles_session_params_set_tile_size(unsigned int client_id, unsigned int session_params_id, unsigned int x, unsigned int y);
CCL_CAPI void __cdecl cycles_session_params_set_tile_order(unsigned int client_id, unsigned int session_params_id, unsigned int tile_order);
CCL_CAPI void __cdecl cycles_session_params_set_start_resolution(unsigned int client_id, unsigned int session_params_id, int start_resolution);
CCL_CAPI void __cdecl cycles_session_params_set_threads(unsigned int client_id, unsigned int session_params_id, unsigned int threads);
CCL_CAPI void __cdecl cycles_session_params_set_display_buffer_linear(unsigned int client_id, unsigned int session_params_id, unsigned int display_buffer_linear);
CCL_CAPI void __cdecl cycles_session_params_set_skip_linear_to_srgb_conversion(unsigned int client_id, unsigned int session_params_id, unsigned int skip_linear_to_srgb_conversion);
CCL_CAPI void __cdecl cycles_session_params_set_cancel_timeout(unsigned int client_id, unsigned int session_params_id, double cancel_timeout);
CCL_CAPI void __cdecl cycles_session_params_set_reset_timeout(unsigned int client_id, unsigned int session_params_id, double reset_timeout);
CCL_CAPI void __cdecl cycles_session_params_set_text_timeout(unsigned int client_id, unsigned int session_params_id, double text_timeout);
CCL_CAPI void __cdecl cycles_session_params_set_shadingsystem(unsigned int client_id, unsigned int session_params_id, unsigned int shadingsystem);
CCL_CAPI void __cdecl cycles_session_params_set_pixel_size(unsigned int client_id, unsigned int session_params_id, unsigned int pixel_size);

/* Create a new scene for specified device. */
CCL_CAPI unsigned int __cdecl cycles_scene_create(unsigned int client_id, unsigned int scene_params_id, unsigned int session_id);
CCL_CAPI void __cdecl cycles_scene_set_background_shader(unsigned int client_id, unsigned int scene_id, unsigned int shader_id);
CCL_CAPI unsigned int __cdecl cycles_scene_get_background_shader(unsigned int client_id, unsigned int scene_id);
CCL_CAPI void __cdecl cycles_scene_set_background_transparent(unsigned int client_id, unsigned int scene_id, unsigned int transparent);
CCL_CAPI void __cdecl cycles_scene_set_background_ao_factor(unsigned int client_id, unsigned int scene_id, float ao_factor);
CCL_CAPI void __cdecl cycles_scene_set_background_ao_distance(unsigned int client_id, unsigned int scene_id, float ao_distance);
CCL_CAPI void __cdecl cycles_scene_set_background_visibility(unsigned int client_id, unsigned int scene_id, unsigned int path_ray_flag);
CCL_CAPI void __cdecl cycles_scene_reset(unsigned int client_id, unsigned int scene_id);
CCL_CAPI bool __cdecl cycles_scene_try_lock(unsigned int client_id, unsigned int scene_id);
CCL_CAPI void __cdecl cycles_scene_lock(unsigned int client_id, unsigned int scene_id);
CCL_CAPI void __cdecl cycles_scene_unlock(unsigned int client_id, unsigned int scene_id);

/* Mesh geometry API */
CCL_CAPI void __cdecl cycles_mesh_set_verts(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id, float *verts, unsigned int vcount);
CCL_CAPI void __cdecl cycles_mesh_set_tris(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id, int *faces, unsigned int fcount, unsigned int shader_id, unsigned int smooth);
CCL_CAPI void __cdecl cycles_mesh_set_triangle(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id, unsigned tri_idx, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int shader_id, unsigned int smooth);
CCL_CAPI void __cdecl cycles_mesh_add_triangle(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int shader_id, unsigned int smooth);
CCL_CAPI void __cdecl cycles_mesh_set_uvs(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id, float *uvs, unsigned int uvcount, const char *uvmap_name);
CCL_CAPI void __cdecl cycles_mesh_set_vertex_normals(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id, float *vnormals, unsigned int vnormalcount);
CCL_CAPI void __cdecl cycles_mesh_set_vertex_colors(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id, float *vcolors, unsigned int vcolorcount);
CCL_CAPI void __cdecl cycles_mesh_set_smooth(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id, unsigned int smooth);
CCL_CAPI void __cdecl cycles_mesh_clear(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id);
CCL_CAPI void __cdecl cycles_mesh_reserve(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id, unsigned vcount, unsigned fcount);
CCL_CAPI void __cdecl cycles_mesh_resize(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id, unsigned vcount, unsigned fcount);
CCL_CAPI void __cdecl cycles_mesh_tag_rebuild(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id);
CCL_CAPI void __cdecl cycles_mesh_set_shader(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id, unsigned int shader_id);
CCL_CAPI void __cdecl cycles_mesh_attr_tangentspace(unsigned int client_id, unsigned int scene_id, unsigned int mesh_id, const char* uvmap_name);

/* Shader API */

#undef TRANSPARENT

// NOTE: keep in sync with available Cycles nodes
enum class shadernode_type : unsigned int {
	BACKGROUND = 0,
	OUTPUT,
	DIFFUSE,
	ANISOTROPIC,
	TRANSLUCENT,
	TRANSPARENT,
	VELVET,
	TOON,
	GLOSSY,
	GLASS,
	REFRACTION,
	HAIR,
	EMISSION,
	AMBIENT_OCCLUSION,
	ABSORPTION_VOLUME,
	SCATTER_VOLUME,
	SUBSURFACE_SCATTERING,
	VALUE,
	COLOR,
	MIX_CLOSURE,
	ADD_CLOSURE,
	INVERT,
	MIX,
	GAMMA,
	WAVELENGTH,
	BLACKBODY,
	CAMERA,
	FRESNEL,
	MATH,
	IMAGE_TEXTURE,
	ENVIRONMENT_TEXTURE,
	BRICK_TEXTURE,
	SKY_TEXTURE,
	CHECKER_TEXTURE,
	NOISE_TEXTURE,
	WAVE_TEXTURE,
	MAGIC_TEXTURE,
	MUSGRAVE_TEXTURE,
	TEXTURE_COORDINATE,
	BUMP,
	RGBTOBW,
	RGBTOLUMINANCE,
	LIGHTPATH,
	LIGHTFALLOFF,
	LAYERWEIGHT,
	GEOMETRYINFO,
	VORONOI_TEXTURE,
	COMBINE_XYZ,
	SEPARATE_XYZ,
	HSV_SEPARATE,
	HSV_COMBINE,
	RGB_SEPARATE,
	RGB_COMBINE,
	MAPPING,
	HOLDOUT,
	HUE_SAT,
	BRIGHT_CONTRAST,
	GRADIENT_TEXTURE,
	COLOR_RAMP,
	VECT_MATH,
	MATRIX_MATH,
	PRINCIPLED_BSDF,
	ATTRIBUTE,
	NORMALMAP,
	WIREFRAME,
	OBJECTINFO,
	TANGENT,
	DISPLACEMENT,
};

CCL_CAPI unsigned int __cdecl cycles_create_shader(unsigned int client_id, unsigned int scene_id);
CCL_CAPI void __cdecl cycles_scene_tag_shader(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, bool use);
CCL_CAPI unsigned int __cdecl cycles_scene_add_shader(unsigned int client_id, unsigned int scene_id, unsigned int shader_id);
/** Set shader_id as default surface shader for scene_id.
 * Note that shader_id is the ID for the shader specific to this scene.
 *
 * The correct ID can be found with cycles_scene_shader_id. The ID is also
 * returned from cycles_scene_add_shader.
 */
CCL_CAPI void __cdecl cycles_scene_set_default_surface_shader(unsigned int client_id, unsigned int scene_id, unsigned int shader_id);
/**
 * Return the current default surface shader id for scene_id.
 */
CCL_CAPI unsigned int __cdecl cycles_scene_get_default_surface_shader(unsigned int client_id, unsigned int scene_id);
CCL_CAPI unsigned int __cdecl cycles_scene_shader_id(unsigned int client_id, unsigned int scene_id, unsigned int shader_id);
CCL_CAPI unsigned int __cdecl cycles_add_shader_node(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, shadernode_type shn_type);
CCL_CAPI void __cdecl cycles_shadernode_set_attribute_int(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, const char* attribute_name, int value);
CCL_CAPI void __cdecl cycles_shadernode_set_attribute_float(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, const char* attribute_name, float value);
CCL_CAPI void __cdecl cycles_shadernode_set_attribute_vec(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, const char* attribute_name, float x, float y, float z);
CCL_CAPI void __cdecl cycles_shadernode_set_enum(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, shadernode_type shn_type, const char* enum_name, int value);
CCL_CAPI void __cdecl cycles_shadernode_texmapping_set_transformation(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, shadernode_type shn_type, int transform_type, float x, float y, float z);
CCL_CAPI void __cdecl cycles_shadernode_texmapping_set_mapping(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, shadernode_type shn_type, ccl::TextureMapping::Mapping x, ccl::TextureMapping::Mapping y, ccl::TextureMapping::Mapping z);
CCL_CAPI void __cdecl cycles_shadernode_texmapping_set_projection(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, shadernode_type shn_type, ccl::TextureMapping::Projection tm_projection);
CCL_CAPI void __cdecl cycles_shadernode_texmapping_set_type(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, shadernode_type shn_type, ccl::NodeMappingType tm_type);

CCL_CAPI void __cdecl cycles_shadernode_set_member_bool(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, shadernode_type shn_type, const char* member_name, bool value);
CCL_CAPI void __cdecl cycles_shadernode_set_member_float(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, shadernode_type shn_type, const char* member_name, float value);
CCL_CAPI void __cdecl cycles_shadernode_set_member_int(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, shadernode_type shn_type, const char* member_name, int value);
CCL_CAPI void __cdecl cycles_shadernode_set_member_vec(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, shadernode_type shn_type, const char* member_name, float x, float y, float z);
CCL_CAPI void __cdecl cycles_shadernode_set_member_string(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, shadernode_type shn_type, const char* member_name, const char* value);
CCL_CAPI void __cdecl cycles_shadernode_set_member_vec4_at_index(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, shadernode_type shn_type, const char* member_name, float x, float y, float z, float w, int index);

CCL_CAPI void __cdecl cycles_shadernode_set_member_float_img(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, shadernode_type shn_type, const char* member_name, const char* img_name, float* img, unsigned int width, unsigned int height, unsigned int depth, unsigned int channels);
CCL_CAPI void __cdecl cycles_shadernode_set_member_byte_img(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int shnode_id, shadernode_type shn_type, const char* member_name, const char* img_name, unsigned char* img, unsigned int width, unsigned int height, unsigned int depth, unsigned int channels);

CCL_CAPI void __cdecl cycles_shader_set_name(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, const char* name);
CCL_CAPI void __cdecl cycles_shader_set_use_mis(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int use_mis);
CCL_CAPI void __cdecl cycles_shader_set_use_transparent_shadow(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int use_transparent_shadow);
CCL_CAPI void __cdecl cycles_shader_set_heterogeneous_volume(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int heterogeneous_volume);
CCL_CAPI void __cdecl cycles_shader_new_graph(unsigned int client_id, unsigned int scene_id, unsigned int shader_id);

CCL_CAPI void __cdecl cycles_shader_connect_nodes(unsigned int client_id, unsigned int scene_id, unsigned int shader_id, unsigned int from_id, const char* from, unsigned int to_id, const char* to);

/***** LIGHTS ****/

/**
 * Different light types available for Cycles
 */
enum class light_type: unsigned int {
	Point = 0,
	Sun, /* = distant, also Hemi */
	Background,
	Area,
	Spot,
	Triangle,
};

CCL_CAPI unsigned int __cdecl cycles_create_light(unsigned int client_id, unsigned int scene_id, unsigned int light_shader_id);
CCL_CAPI void __cdecl cycles_light_set_type(unsigned int client_id, unsigned int scene_id, unsigned int light_id, light_type type);
CCL_CAPI void __cdecl cycles_light_set_angle(unsigned int client_id, unsigned int scene_id, unsigned int light_id, float angle);
CCL_CAPI void __cdecl cycles_light_set_spot_angle(unsigned int client_id, unsigned int scene_id, unsigned int light_id, float spot_angle);
CCL_CAPI void __cdecl cycles_light_set_spot_smooth(unsigned int client_id, unsigned int scene_id, unsigned int light_id, float spot_smooth);
CCL_CAPI void __cdecl cycles_light_set_cast_shadow(unsigned int client_id, unsigned int scene_id, unsigned int light_id, unsigned int cast_shadow);
CCL_CAPI void __cdecl cycles_light_set_use_mis(unsigned int client_id, unsigned int scene_id, unsigned int light_id, unsigned int use_mis);
CCL_CAPI void __cdecl cycles_light_set_samples(unsigned int client_id, unsigned int scene_id, unsigned int light_id, unsigned int samples);
CCL_CAPI void __cdecl cycles_light_set_max_bounces(unsigned int client_id, unsigned int scene_id, unsigned int light_id, unsigned int max_bounces);
CCL_CAPI void __cdecl cycles_light_set_map_resolution(unsigned int client_id, unsigned int scene_id, unsigned int light_id, unsigned int map_resolution);
CCL_CAPI void __cdecl cycles_light_set_sizeu(unsigned int client_id, unsigned int scene_id, unsigned int light_id, float sizeu);
CCL_CAPI void __cdecl cycles_light_set_sizev(unsigned int client_id, unsigned int scene_id, unsigned int light_id, float sizev);
CCL_CAPI void __cdecl cycles_light_set_axisu(unsigned int client_id, unsigned int scene_id, unsigned int light_id, float axisux, float axisuy, float axisuz);
CCL_CAPI void __cdecl cycles_light_set_axisv(unsigned int client_id, unsigned int scene_id, unsigned int light_id, float axisvx, float axisvy, float axisvz);
CCL_CAPI void __cdecl cycles_light_set_size(unsigned int client_id, unsigned int scene_id, unsigned int light_id, float size);
CCL_CAPI void __cdecl cycles_light_set_dir(unsigned int client_id, unsigned int scene_id, unsigned int light_id, float dirx, float diry, float dirz);
CCL_CAPI void __cdecl cycles_light_set_co(unsigned int client_id, unsigned int scene_id, unsigned int light_id, float cox, float coy, float coz);
CCL_CAPI void __cdecl cycles_light_tag_update(unsigned int client_id, unsigned int scene_id, unsigned int light_id);

CCL_CAPI void __cdecl cycles_film_set_exposure(unsigned int client_id, unsigned int scene_id, float exposure);
CCL_CAPI void __cdecl cycles_film_set_filter(unsigned int client_id, unsigned int scene_id, unsigned int filter_type, float filter_width);
CCL_CAPI void __cdecl cycles_film_set_use_sample_clamp(unsigned int client_id, unsigned int scene_id, bool use_sample_clamp);
CCL_CAPI void __cdecl cycles_film_tag_update(unsigned int client_id, unsigned int scene_id);

CCL_CAPI void __cdecl cycles_f4_add(ccl::float4 a, ccl::float4 b, ccl::float4& res);
CCL_CAPI void __cdecl cycles_f4_sub(ccl::float4 a, ccl::float4 b, ccl::float4& res);
CCL_CAPI void __cdecl cycles_f4_mul(ccl::float4 a, ccl::float4 b, ccl::float4& res);
CCL_CAPI void __cdecl cycles_f4_div(ccl::float4 a, ccl::float4 b, ccl::float4& res);

CCL_CAPI void __cdecl cycles_tfm_inverse(const ccl::Transform& t, ccl::Transform& res);
CCL_CAPI void __cdecl cycles_tfm_lookat(const ccl::float3& position, const ccl::float3& look, const ccl::float3& up, ccl::Transform& res);
CCL_CAPI void __cdecl cycles_tfm_rotate_around_axis(float angle, const ccl::float3& axis, ccl::Transform& res);

CCL_CAPI void __cdecl cycles_apply_gamma_to_byte_buffer(unsigned char* rgba_buffer, size_t size_in_bytes, float gamma);
CCL_CAPI void __cdecl cycles_apply_gamma_to_float_buffer(float* rgba_buffer, size_t size_in_bytes, float gamma);

#ifdef __cplusplus
}
#endif

#endif
