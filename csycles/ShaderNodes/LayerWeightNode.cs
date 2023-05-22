/**
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
	public class LayerWeightInputs : Inputs
	{
		public FloatSocket Blend { get; set; }
		public VectorSocket Normal { get; set; }

		public LayerWeightInputs(ShaderNode parentNode)
		{
			Blend = new FloatSocket(parentNode, "Blend");
			AddSocket(Blend);
			Normal = new VectorSocket(parentNode, "Normal");
			AddSocket(Normal);
		}
	}

	public class LayerWeightOutputs : Outputs
	{
		public FloatSocket Fresnel { get; set; }
		public FloatSocket Facing { get; set; }

		public LayerWeightOutputs(ShaderNode parentNode)
		{
			Fresnel = new FloatSocket(parentNode, "Fresnel");
			AddSocket(Fresnel);
			Facing = new FloatSocket(parentNode, "Facing");
			AddSocket(Facing);
		}
	}

	[ShaderNode("layer_weight")]
	public class LayerWeightNode : ShaderNode
	{
		public LayerWeightInputs ins => (LayerWeightInputs)inputs;
		public LayerWeightOutputs outs => (LayerWeightOutputs)outputs;

		public LayerWeightNode(Shader shader) : this(shader, "a layerweight node") { }
		public LayerWeightNode(Shader shader, string name)
			: base(shader, true)
		{
			inputs = new LayerWeightInputs(this);
			outputs = new LayerWeightOutputs(this);

			ins.Normal.Value = new float4(0.0f);
			ins.Blend.Value = 0.5f;
		}

		internal override void ParseXml(XmlReader xmlNode)
		{
			Utilities.Instance.get_float(ins.Blend, xmlNode.GetAttribute("blend"));
			Utilities.Instance.get_float4(ins.Normal, xmlNode.GetAttribute("normal"));
		}
	}
}
