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
using ccl.ShaderNodes.Sockets;
using ccl.Attributes;
using System.Xml;
using System.Drawing;

namespace ccl.ShaderNodes
{
	/// <summary>
	/// EnvironmentTextureNode input sockets
	/// </summary>
	public class EnvironmentTextureInputs : Inputs
	{
		/// <summary>
		/// EnvironmentTextureNode vector input 
		/// </summary>
		public Float4Socket Vector { get; set; }

		internal EnvironmentTextureInputs(ShaderNode parentNode)
		{
			Vector = new Float4Socket(parentNode, "Vector");
			AddSocket(Vector);
		}
	}

	/// <summary>
	/// EnvironmentTextureNode output sockets
	/// </summary>
	public class EnvironmentTextureOutputs : Outputs
	{
		/// <summary>
		/// EnvironmentTextureNode color output
		/// </summary>
		public Float4Socket Color { get; set; }
		/// <summary>
		/// EnvironmentTextureNode alpha output
		/// </summary>
		public FloatSocket Alpha { get; set; }

		internal EnvironmentTextureOutputs(ShaderNode parentNode)
		{
			Color = new Float4Socket(parentNode, "Color");
			AddSocket(Color);
			Alpha = new FloatSocket(parentNode, "Alpha");
			AddSocket(Alpha);
		}
	}

	/// <summary>
	/// EnvironmentTextureNode
	/// </summary>
	[ShaderNode("environment_texture")]
	public class EnvironmentTextureNode : TextureNode
	{
		/// <summary>
		/// EnvironmentTextureNode input sockets
		/// </summary>
		public EnvironmentTextureInputs ins { get { return (EnvironmentTextureInputs)inputs; } }
		/// <summary>
		/// EnvironmentTextureNode output sockets
		/// </summary>
		public EnvironmentTextureOutputs outs { get { return (EnvironmentTextureOutputs)outputs; } }

		public EnvironmentTextureNode() : this("an env texture node") { }

		/// <summary>
		/// Create an EnvironmentTextureNode
		/// </summary>
		public EnvironmentTextureNode(string name) :
			base(ShaderNodeType.EnvironmentTexture, name)
		{

			inputs = new EnvironmentTextureInputs(this);
			outputs = new EnvironmentTextureOutputs(this);

			Interpolation = InterpolationType.Linear;
			ColorSpace = TextureColorSpace.None;
			Projection = EnvironmentProjection.Equirectangular;
		}

		/// <summary>
		/// Set to true if image data is to be interpreted as linear.
		/// </summary>
		public bool IsLinear { get; set; }
		/// <summary>
		/// Color space to operate in
		/// </summary>
		public TextureColorSpace ColorSpace { get; set; }
		/// <summary>
		/// Get or set environment projection
		/// </summary>
		public EnvironmentProjection Projection { get; set; }
		/// <summary>
		/// EnvironmentTexture texture interpolation
		/// </summary>
		public InterpolationType Interpolation { get; set; }
		/// <summary>
		/// Get or set image name
		/// </summary>
		public string Filename { get; set; }

		/// <summary>
		/// Get or set the float data for image. Use for HDR images
		/// </summary>
		public float[] FloatImage { set; get; }
		
		/// <summary>
		/// Get or set the byte data for image
		/// </summary>
		public byte[] ByteImage { set; get; }

		/// <summary>
		/// Get or set image resolution width
		/// </summary>
		public uint Width { get; set; }
		/// <summary>
		/// Get or set image resolution height
		/// </summary>
		public uint Height { get; set; }

		private string GetProjectionString(EnvironmentProjection projection)
		{

			switch (projection)
			{
				case EnvironmentProjection.Equirectangular:
					return "Equirectangular";
				case EnvironmentProjection.MirrorBall:
					return "Mirror Ball";
				case EnvironmentProjection.Wallpaper:
					return "Wallpaper";
			}

			return "Equirectangular";
		}

		internal override void SetEnums(uint clientId, uint shaderId)
		{
			var projection = GetProjectionString(Projection);
			var colspace = ColorSpace == TextureColorSpace.Color ? "Color" : "None";
			CSycles.shadernode_set_enum(clientId, shaderId, Id, Type, "projection", projection);
			CSycles.shadernode_set_enum(clientId, shaderId, Id, Type, "color_space", colspace);
		}

		internal override void SetDirectMembers(uint clientId, uint shaderId)
		{
			CSycles.shadernode_set_member_bool(clientId, shaderId, Id, Type, "is_linear", IsLinear);
			CSycles.shadernode_set_member_int(clientId, shaderId, Id, Type, "interpolation", (int)Interpolation);
			if (FloatImage != null)
			{
				var flimg = FloatImage;
				CSycles.shadernode_set_member_float_img(clientId, shaderId, Id, Type, "builtin-data", Filename ?? String.Format("{0}-{0}-{0}", clientId, shaderId, Id), ref flimg, Width, Height, 1, 4);
			}
			else if (ByteImage != null)
			{
				var bimg = ByteImage;
				CSycles.shadernode_set_member_byte_img(clientId, shaderId, Id, Type, "builtin-data", Filename ?? String.Format("{0}-{0}-{0}", clientId, shaderId, Id), ref bimg, Width, Height, 1, 4);
			}
		}

		internal override void ParseXml(XmlReader xmlNode)
		{
			var imgsrc = xmlNode.GetAttribute("src");
			if (!string.IsNullOrEmpty(imgsrc) && System.IO.File.Exists(imgsrc))
			{
				using (var bmp = new Bitmap(imgsrc))
				{
					var l = bmp.Width * bmp.Height * 4;
					var bmpdata = new float[l];
					for (var x = 0; x < bmp.Width; x++)
					{
						for (var y = 0; y < bmp.Height; y++)
						{
							var pos = y * bmp.Width * 4 + x * 4;
							var pixel = bmp.GetPixel(x, y);
							bmpdata[pos] = pixel.R / 255.0f;
							bmpdata[pos + 1] = pixel.G / 255.0f;
							bmpdata[pos + 2] = pixel.B / 255.0f;
							bmpdata[pos + 3] = pixel.A / 255.0f;
						}
					}
					FloatImage = bmpdata;
					Width = (uint)bmp.Width;
					Height = (uint)bmp.Height;
					Filename = imgsrc;
				}
			}
		}
	}
}
