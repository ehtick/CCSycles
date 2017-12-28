﻿using System.Runtime.InteropServices;

namespace ccl
{
	public partial class CSycles
	{
#region camera
		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_size(uint clientId, uint sceneId, uint width, uint height);
		public static void camera_set_size(uint clientId, uint sceneId, uint width, uint height)
		{
			cycles_camera_set_size(clientId, sceneId, width, height);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern uint cycles_camera_get_width(uint clientId, uint sceneId);
		public static uint camera_get_width(uint clientId, uint sceneId)
		{
			return cycles_camera_get_width(clientId, sceneId);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern uint cycles_camera_get_height(uint clientId, uint sceneId);
		public static uint camera_get_height(uint clientId, uint sceneId)
		{
			return cycles_camera_get_height(clientId, sceneId);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_matrix(uint clientId, uint sceneId,
			float a, float b, float c, float d,
			float e, float f, float g, float h,
			float i, float j, float k, float l,
			float m, float n, float o, float p);
		public static void camera_set_matrix(uint clientId, uint sceneId, Transform t)
		{
			cycles_camera_set_matrix(clientId, sceneId,
				t.x.x, t.x.y, t.x.z, t.x.w,
				t.y.x, t.y.y, t.y.z, t.y.w,
				t.z.x, t.z.y, t.z.z, t.z.w,
				t.w.x, t.w.y, t.w.z, t.w.w);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_type(uint clientId, uint sceneId, uint type);
		public static void camera_set_type(uint clientId, uint sceneId, CameraType type)
		{
			cycles_camera_set_type(clientId, sceneId, (uint)type);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_panorama_type(uint clientId, uint sceneId, uint type);
		public static void camera_set_panorama_type(uint clientId, uint sceneId, PanoramaType type)
		{
			cycles_camera_set_panorama_type(clientId, sceneId, (uint)type);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_compute_auto_viewplane(uint clientId, uint sceneId);
		public static void camera_compute_auto_viewplane(uint clientId, uint sceneId)
		{
			cycles_camera_compute_auto_viewplane(clientId, sceneId);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_viewplane(uint clientId, uint sceneId, float left, float right, float top, float bottom);
		public static void camera_set_viewplane(uint clientId, uint sceneId, float left, float right, float top, float bottom)
		{
			cycles_camera_set_viewplane(clientId, sceneId, left, right, top, bottom);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_update(uint clientId, uint sceneId);
		public static void camera_update(uint clientId, uint sceneId)
		{
			cycles_camera_update(clientId, sceneId);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_fov(uint clientId, uint sceneId, float fov);
		public static void camera_set_fov(uint clientId, uint sceneId, float fov)
		{
			cycles_camera_set_fov(clientId, sceneId, fov);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_sensor_width(uint clientId, uint sceneId, float sensorWidth);
		public static void camera_set_sensor_width(uint clientId, uint sceneId, float sensorWidth)
		{
			cycles_camera_set_sensor_width(clientId, sceneId, sensorWidth);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_sensor_height(uint clientId, uint sceneId, float sensorHeight);
		public static void camera_set_sensor_height(uint clientId, uint sceneId, float sensorHeight)
		{
			cycles_camera_set_sensor_height(clientId, sceneId, sensorHeight);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_nearclip(uint clientId, uint sceneId, float nearclip);
		public static void camera_set_nearclip(uint clientId, uint sceneId, float nearclip)
		{
			cycles_camera_set_nearclip(clientId, sceneId, nearclip);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_farclip(uint clientId, uint sceneId, float farclip);
		public static void camera_set_farclip(uint clientId, uint sceneId, float farclip)
		{
			cycles_camera_set_farclip(clientId, sceneId, farclip);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_aperturesize(uint clientId, uint sceneId, float aperturesize);
		public static void camera_set_aperturesize(uint clientId, uint sceneId, float aperturesize)
		{
			cycles_camera_set_aperturesize(clientId, sceneId, aperturesize);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_aperture_ratio(uint clientId, uint sceneId, float apertureRatio);
		public static void camera_set_aperture_ratio(uint clientId, uint sceneId, float apertureRatio)
		{
			cycles_camera_set_aperture_ratio(clientId, sceneId, apertureRatio);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_blades(uint clientId, uint sceneId, uint blades);
		public static void camera_set_blades(uint clientId, uint sceneId, uint blades)
		{
			cycles_camera_set_blades(clientId, sceneId, blades);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_bladesrotation(uint clientId, uint sceneId, float bladesrotation);
		public static void camera_set_bladesrotation(uint clientId, uint sceneId, float bladesrotation)
		{
			cycles_camera_set_bladesrotation(clientId, sceneId, bladesrotation);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_shuttertime(uint clientId, uint sceneId, float shuttertime);
		public static void camera_set_shuttertime(uint clientId, uint sceneId, float shuttertime)
		{
			cycles_camera_set_shuttertime(clientId, sceneId, shuttertime);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_focaldistance(uint clientId, uint sceneId, float focaldistance);
		public static void camera_set_focaldistance(uint clientId, uint sceneId, float focaldistance)
		{
			cycles_camera_set_focaldistance(clientId, sceneId, focaldistance);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_fisheye_fov(uint clientId, uint sceneId, float fisheyeFov);
		public static void camera_set_fisheye_fov(uint clientId, uint sceneId, float fisheyeFov)
		{
			cycles_camera_set_fisheye_fov(clientId, sceneId, fisheyeFov);
		}

		[DllImport(Constants.ccycles, SetLastError = false, CallingConvention = CallingConvention.Cdecl)]
		private static extern void cycles_camera_set_fisheye_lens(uint clientId, uint sceneId, float fisheyeLens);
		public static void camera_set_fisheye_lens(uint clientId, uint sceneId, float fisheyeLens)
		{
			cycles_camera_set_fisheye_lens(clientId, sceneId, fisheyeLens);
		}
#endregion
	}
}
