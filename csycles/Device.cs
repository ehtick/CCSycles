﻿/**
Copyright 2014 Robert McNeel and Associates

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

using System;
using System.Collections.Generic;
using System.Linq;

namespace ccl
{
	/// <summary>
	/// Representation of a Cycles rendering device
	/// </summary>
	public class Device
	{
		/// <summary>
		/// Get the numerical ID for this device
		/// </summary>
		public uint Id { get; private set; }
		/// <summary>
		/// Get the Cycles description for the device
		/// </summary>
		public string Description { get; private set; }
		/// <summary>
		/// The name for the device
		/// </summary>
		public string Name { get; private set; }
		/// <summary>
		/// Get the Cycles num for the device
		/// </summary>
		public uint Num { get; private set; }
		/// <summary>
		/// Get the device type
		/// </summary>
		public DeviceType Type { get; private set; }
		/// <summary>
		/// True if this device supports advanced shading
		/// </summary>
		public bool AdvancedShading { get; private set; }
		/// <summary>
		/// True if this device is used as a display device
		/// </summary>
		public bool DisplayDevice { get; private set; }
		/// <summary>
		/// True if this device supports packed images
		/// </summary>
		public bool PackImages { get; private set; }
		/// <summary>
		/// True if this is a CUDA device
		/// </summary>
		public bool IsCuda { get { return Type == DeviceType.CUDA; } }

		/// <summary>
		/// True if this device is an OpenCL device
		/// </summary>
		public bool IsOpenCl { get { return Type == DeviceType.OpenCL; } }

		/// <summary>
		/// True if this device is a CPU
		/// </summary>
		public bool IsCpu { get { return Type == DeviceType.CPU; } }

		/// <summary>
		/// True if this device is a GPU
		/// </summary>
		public bool IsGpu { get { return !IsCpu; } }

		/// <summary>
		/// True if this is a Multi CUDA device
		/// </summary>
		public bool IsMultiCuda { get { return Type == DeviceType.Multi && Name.Contains("CUDA"); } }

		/// <summary>
		/// True if this is a Multi OpenCL device
		/// </summary>
		public bool IsMultiOpenCl { get { return Type == DeviceType.Multi && Name.Contains("OPENCL"); } }

		/// <summary>
		/// String representation of this device
		/// </summary>
		/// <returns>String representation of this device</returns>
		public override string ToString()
		{
			return $"{base.ToString()}: {Description} ({Type}), Id {Id} Num {Num} Name {Name} DisplayDevice {DisplayDevice} AdvancedShading {AdvancedShading} PackImages {PackImages}";
		}

		/// <summary>
		/// Get the default device (CPU)
		/// </summary>
		/// <returns>The default device</returns>
		static public Device Default
		{
			get
			{
				return GetDevice(0);
			}
		}

		/// <summary>
		/// Get capabilities of all devices that Cycles can see.
		/// </summary>
		static public string Capabilities
		{
			get
			{
				return CSycles.device_capabilities();
			}
		}

		/// <summary>
		/// Get a device by using GetDevice(int idx). Constructor is private.
		/// </summary>
		private Device() { }

		/// <summary>
		/// Get the number of available Cycles render devices
		/// </summary>
		/// <returns></returns>
		static public uint Count
		{
			get
			{
				return CSycles.number_devices();
			}
		}

		/// <summary>
		/// True if any of the available devices is a CUDA device
		/// </summary>
		/// <returns></returns>
		static public bool CudaAvailable()
		{
			return CSycles.number_cuda_devices() > 0;
		}

		/// <summary>
		/// Enumerate over available devices.
		/// </summary>
		static public IEnumerable<Device> Devices
		{
			get
			{
				for (var i = 0; i < Count; i++)
				{
					yield return GetDevice(i);
				}
			}
		}

		/// <summary>
		/// Returns the first cuda device if it exists,
		/// the default rendering device (CPU) if not.
		/// </summary>
		static public Device FirstCuda
		{
			get
			{
				var d = (from device in Devices
					where device.IsCuda || device.IsMultiCuda
					select device).FirstOrDefault();
				return d ?? Default;
			}
		}

		/// <summary>
		/// Returns the first openCL device if it exists,
		/// the default rendering device (CPU) if not.
		/// </summary>
		static public Device FirstOpenCL
		{
			get
			{
				var d = (from device in Devices
					where device.IsOpenCl|| device.IsMultiOpenCl
					select device).FirstOrDefault();
				return d ?? Default;
			}
		}
		
		/// <summary>
		/// Returns the first Multi OpenCL device if it exists,
		/// the default rendering device (CPU) if not.
		/// </summary>
		static public Device FirstMultiOpenCL
		{
			get
			{
				var d = (from device in Devices
					where device.IsMultiOpenCl
					select device).FirstOrDefault();
				return d ?? Default;
			}
		}

		/// <summary>
		/// Get the first GPU.
		/// </summary>
		static public Device FirstGpu
		{
			get
			{
				var d = (from device in Devices
					where device.IsGpu
					select device).FirstOrDefault();
				return d ?? Default;

			}
		}


		/// <summary>
		/// Get the device with specified index
		/// </summary>
		/// <param name="idx"></param>
		/// <returns></returns>
		static public Device GetDevice(int idx)
		{
			return new Device
			{
				Id = (uint)idx,
				Description = CSycles.device_decription(idx),
				Name = CSycles.DeviceId(idx),
				Num = CSycles.device_num(idx),
				Type = CSycles.device_type(idx),
				AdvancedShading = CSycles.device_advanced_shading(idx),
				DisplayDevice = CSycles.device_display_device(idx),
				PackImages = CSycles.device_pack_images(idx)
			};
		}
	}
}
