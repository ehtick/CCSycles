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

using System.Xml;
using ccl.ShaderNodes.Sockets;
using ccl.Attributes;

namespace ccl.ShaderNodes
{
	public class NoiseInputs : Inputs
	{
		public VectorSocket Vector { get; set; }
		public FloatSocket Scale { get; set; }
		public FloatSocket Detail { get; set; }
		public FloatSocket Distortion { get; set; }

		public NoiseInputs(ShaderNode parentNode)
		{
			Vector = new VectorSocket(parentNode, "Vector");
			AddSocket(Vector);
			Scale = new FloatSocket(parentNode, "Scale");
			AddSocket(Scale);
			Detail = new FloatSocket(parentNode, "Detail");
			AddSocket(Detail);
			Distortion = new FloatSocket(parentNode, "Distortion");
			AddSocket(Distortion);
		}
	}

	public class NoiseOutputs : Outputs
	{
		public ColorSocket Color { get; set; }
		public FloatSocket Fac { get; set; }

		public NoiseOutputs(ShaderNode parentNode)
		{
			Color = new ColorSocket(parentNode, "Color");
			AddSocket(Color);
			Fac = new FloatSocket(parentNode, "Fac");
			AddSocket(Fac);
		}
	}

	[ShaderNode("noise_texture")]
	public class NoiseTexture : TextureNode
	{
		public NoiseInputs ins => (NoiseInputs)inputs;
		public NoiseOutputs outs => (NoiseOutputs)outputs;

		public NoiseTexture() : this("a noise texture") { }
		public NoiseTexture(string name)
			: base(ShaderNodeType.NoiseTexture, name)
		{
			inputs = new NoiseInputs(this);
			outputs = new NoiseOutputs(this);

			ins.Scale.Value = 1.0f;
			ins.Detail.Value = 2.0f;
			ins.Distortion.Value = 0.0f;
		}

		internal override void ParseXml(XmlReader xmlNode)
		{
			Utilities.Instance.get_float4(ins.Vector, xmlNode.GetAttribute("vector"));
			Utilities.Instance.get_float(ins.Detail, xmlNode.GetAttribute("detail"));
			Utilities.Instance.get_float(ins.Distortion, xmlNode.GetAttribute("distortion"));
			Utilities.Instance.get_float(ins.Scale, xmlNode.GetAttribute("scale"));
		}
	}
}
