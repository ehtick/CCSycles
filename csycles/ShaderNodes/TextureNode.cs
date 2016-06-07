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

using ccl.Attributes;

namespace ccl.ShaderNodes
{
	/// <summary>
	/// Base texture node
	/// </summary>
	[ShaderNode("texture_node_base", true)]
	public class TextureNode : ShaderNode
	{
		public enum TextureColorSpace
		{
			None,
			Color,
		}

		public enum TextureProjection
		{
			Flat,
			Box,
			Sphere,
			Tube,
		}

		public enum TextureExtension
		{
			Repeat,
			Extend,
			Clip,
		}

		public enum EnvironmentProjection
		{
			Equirectangular,
			MirrorBall,
			Wallpaper
		}

		internal TextureNode(ShaderNodeType type) :
			base(type) { }

		internal TextureNode(ShaderNodeType type, string name) :
			base(type, name) { }
	}
}
