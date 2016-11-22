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
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Xml;
using ccl.ShaderNodes.Sockets;
using ccl.Attributes;

namespace ccl.ShaderNodes
{
	using cclext;
	/// <summary>
	/// Base class for shader nodes.
	/// </summary>
	[ShaderNode("shadernode base", true)]
	public class ShaderNode
	{
		private static int _runtimeSerial = 0;

		private int id;
		/// <summary>
		/// Set a name for this node
		/// </summary>
		public string Name { get; }

		/// <summary>
		/// Get name that can be used as variable name
		/// </summary>
		public virtual string VariableName
		{
			get
			{
				var s = $"{ShaderNodeTypeCodeName}{id}";
				return Extensions.FirstCharacterToLower(s);
			}
		}

		/// <summary>
		/// Get the node ID. This is set when created in Cycles.
		/// </summary>
		public uint Id { get; internal set; }
		/// <summary>
		/// Get the shader node type. Set in the constructor.
		/// </summary>
		public ShaderNodeType Type { get; }

		/// <summary>
		/// Get the XML name of the node type as string.
		/// </summary>
		public string ShaderNodeTypeName
		{
			get
			{
				var t = GetType();
				var attr = t.GetCustomAttributes(typeof (ShaderNodeAttribute), false)[0] as ShaderNodeAttribute;
				return attr.Name;
			}
		}

		public string ShaderNodeTypeCodeName
		{
			get {
				var t = GetType();
				return t.Name;
			}
		}

		/// <summary>
		/// Generic access to input sockets.
		/// </summary>
		public Inputs inputs { get; set; }
		/// <summary>
		/// Generic access to output sockets.
		/// </summary>
		public Outputs outputs { get; set; }

		/// <summary>
		/// Create node of type ShaderNodeType type
		/// </summary>
		/// <param name="type"></param>
		internal ShaderNode(ShaderNodeType type) : this(type, String.Empty)
		{
		}

		/// <summary>
		/// Create node of type ShaderNodeType and with given name
		/// </summary>
		/// <param name="type"></param>
		/// <param name="name"></param>
		internal ShaderNode(ShaderNodeType type, string name)
		{
			id = _runtimeSerial++;
			Type = type;
			Name = name;
		}

		/// <summary>
		/// A node deriving from ShaderNode should override this if
		/// it has enumerations that need to be committed to Cycles
		/// </summary>
		/// <param name="clientId"></param>
		/// <param name="shaderId"></param>
		virtual internal void SetEnums(uint clientId, uint shaderId)
		{
			// do nothing
		}

		/// <summary>
		/// A node deriving from ShaderNode should override this if
		/// it has direct members that need to be committed to Cycles
		/// </summary>
		/// <param name="clientId"></param>
		/// <param name="shaderId"></param>
		virtual internal void SetDirectMembers(uint clientId, uint shaderId)
		{
			// do nothing
		}

		internal void SetSockets(uint clientId, uint shaderId)
		{
			/* set node attributes */
			if (inputs != null)
			{
				foreach (var socket in inputs.Sockets)
				{
					var float_socket = socket as FloatSocket;
					if (float_socket != null)
					{
						CSycles.shadernode_set_attribute_float(clientId, shaderId, Id, float_socket.Name, float_socket.Value);
					}
					var int_socket = socket as IntSocket;
					if (int_socket != null)
					{
						CSycles.shadernode_set_attribute_int(clientId, shaderId, Id, int_socket.Name, int_socket.Value);
					}
					var string_socket = socket as StringSocket;
					if (string_socket != null)
					{
						CSycles.shadernode_set_attribute_string(clientId, shaderId, Id, socket.Name, string_socket.Value);
					}
					var float4_socket = socket as Float4Socket;
					if (float4_socket != null)
					{
						CSycles.shadernode_set_attribute_vec(clientId, shaderId, Id, float4_socket.Name, float4_socket.Value);
					}
				}
			}
		}

		public override string ToString()
		{
			var str = $"{Name} ({Type})";
			return str;
		}

		/// <summary>
		/// Implement ParseXml to support proper XMl support.
		/// </summary>
		/// <param name="xmlNode"></param>
		virtual internal void ParseXml(XmlReader xmlNode)
		{
		}

		public virtual string CreateXmlAttributes()
		{
			return "";
		}

		public virtual string CreateXml()
		{
			var nfi = Utilities.Instance.NumberFormatInfo;
			var xml = new StringBuilder($"<{ShaderNodeTypeName} name=\"{Name}\" ", 1024);


			foreach (var inp in inputs.Sockets)
			{
				var fs = inp as FloatSocket;
				if (fs != null)
				{
					xml.AppendFormat(nfi, " {0}=\"{1}\"", fs.XmlName, fs.Value);
					continue;
				}
				var ints = inp as IntSocket;
				if (ints != null)
				{
					xml.AppendFormat(nfi, " {0}=\"{1}\"", ints.XmlName, ints.Value);
					continue;
				}
				var cols = inp as ColorSocket;
				if (cols != null)
				{
					xml.AppendFormat(nfi, " {0}=\"{1} {2} {3} {4}\"", cols.XmlName, cols.Value.x, cols.Value.y, cols.Value.z, cols.Value.w);
					continue;
				}
				var vec = inp as VectorSocket;
				if (vec != null)
				{
					xml.AppendFormat(nfi, " {0}=\"{1} {2} {3} {4}\"", vec.XmlName, vec.Value.x, vec.Value.y, vec.Value.z, vec.Value.w);
					continue;
				}
				var f4s = inp as Float4Socket;
				if (f4s != null)
				{
					xml.AppendFormat(nfi, " {0}=\"{1} {2} {3} {4}\"", f4s.XmlName, f4s.Value.x, f4s.Value.y, f4s.Value.z, f4s.Value.w);
					continue;
				}
				var strs = inp as StringSocket;
				if (strs != null)
				{
					xml.AppendFormat(nfi, " {0}=\"{1}\"", strs.XmlName, strs.Value);
				}
			}

			xml.Append(CreateXmlAttributes());

			xml.Append(" />");

			return xml.ToString();
		}

		public virtual string CreateConnectXml()
		{
			var sb = inputs.Sockets.Aggregate(new StringBuilder("", 1024), (current, inp) => current.Append($"{inp.ConnectTag}\n"));
			return sb.ToString();
		}

		public virtual string CreateCodeAttributes()
		{
			var nfi = Utilities.Instance.NumberFormatInfo;
			var attr = new StringBuilder(1024); 
			if (inputs.Sockets.Any())
			{
				foreach (var inp in inputs.Sockets)
				{
					var fs = inp as FloatSocket;
					if (fs != null)
					{
						attr.AppendFormat(nfi, " {0}.ins.{1}.Value = {2}f;", VariableName, fs.CodeName, fs.Value);
						continue;
					}
					var ints = inp as IntSocket;
					if (ints != null)
					{
						attr.AppendFormat(nfi, " {0}.ins.{1}.Value = {2};", VariableName, ints.CodeName, ints.Value);
						continue;
					}
					var cols = inp as ColorSocket;
					if (cols != null)
					{
						attr.AppendFormat(nfi, " {0}.ins.{1}.Value = new float4({2}f, {3}f, {4}f, {5}f);", VariableName, cols.CodeName, cols.Value.x, cols.Value.y, cols.Value.z, cols.Value.w);
						continue;
					}
					var vec = inp as VectorSocket;
					if (vec != null)
					{
						attr.AppendFormat(nfi, " {0}.ins.{1}.Value = new float4({2}f, {3}f, {4}f, {5}f);", VariableName, vec.CodeName, vec.Value.x, vec.Value.y, vec.Value.z, vec.Value.w);
						continue;
					}
					var f4s = inp as Float4Socket;
					if (f4s != null)
					{
						attr.AppendFormat(nfi," {0}.ins.{1}.Value = new float4({2}f, {3}f, {4}f, {5}f);", VariableName, f4s.CodeName, f4s.Value.x, f4s.Value.y, f4s.Value.z, f4s.Value.w);
						continue;
					}
					var strs = inp as StringSocket;
					if (strs != null)
					{
						attr.AppendFormat(nfi, " {0}.ins.{1}.Value = \"{2}\";", VariableName, strs.CodeName, strs.Value);
					}
					attr.AppendLine();
				}
				attr.AppendLine();
			}
			return attr.ToString();
		}

		public virtual string CreateCode()
		{
			var cs = new StringBuilder($"var {VariableName} = new {ShaderNodeTypeCodeName}(\"{Name}\");", 1024);

			cs.Append(CreateCodeAttributes());

			return cs.ToString();
		}

		public virtual string CreateConnectCode()
		{
			var sb = inputs.Sockets.Aggregate(new StringBuilder("", 1024), (current, inp) => current.Append($"{inp.ConnectCode}\n"));
			return sb.ToString();
		}

		public virtual string CreateAddToShaderCode(string shader)
		{
			var sb = new StringBuilder(1024);

			sb.Append($"{shader}.AddNode({VariableName});");

			return sb.ToString();
		}
	}
}
