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

using ccl.ShaderNodes.Sockets;
using ccl.Attributes;
using System.Text;
using System.Linq;

namespace ccl.ShaderNodes
{
	/// <summary>
	/// OutputNode input sockets
	/// </summary>
	public class OutputInputs : Inputs
	{
		/// <summary>
		/// Surface shading socket. Plug Background here for world shaders
		/// </summary>
		public ClosureSocket Surface { get; set; }
		public ClosureSocket Volume { get; set; }
		/// <summary>
		/// Only useful for material output nodes
		/// </summary>
		public FloatSocket Displacement { get; set; }

		internal OutputInputs(ShaderNode parentNode)
		{
			Surface = new ClosureSocket(parentNode, "Surface");
			AddSocket(Surface);
			Volume = new ClosureSocket(parentNode, "Volume");
			AddSocket(Volume);
			Displacement = new FloatSocket(parentNode, "Displacement");
			AddSocket(Displacement);
		}
	}

	public class OutputOutputs : Outputs
	{
		internal OutputOutputs(ShaderNode parentNode)
		{
			
		}
	}

	/// <summary>
	/// The final output shader node for shaders.
	/// </summary>
	[ShaderNode("output")]
	public class OutputNode : ShaderNode
	{
		/// <summary>
		/// For code generation always return "shader.Output" so automatic
		/// code generation gives result that can be directly copy/pasted.
		/// </summary>
		public override string VariableName => "shader.Output";

		public OutputInputs ins => (OutputInputs) inputs;
		public OutputOutputs outs => (OutputOutputs) outputs;

		public OutputNode() : this("output") { }
		public OutputNode(string name) :
			base(ShaderNodeType.Output, "output")
		{
			inputs = new OutputInputs(this);
			outputs = new OutputOutputs(this);
		}
	}

}
