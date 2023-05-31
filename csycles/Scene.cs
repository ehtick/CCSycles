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

using System.Collections.Generic;
using System.Linq;
using System;

namespace ccl
{
	/// <summary>
	/// The Cycles scene representation
	/// </summary>
	public class Scene
	{
		/// <summary>
		/// Get the ID of the created Session as given by CCycles
		/// </summary>
		public IntPtr Id { get; private set; }

		/// <summary>
		/// Access to the Camera for this Session
		/// </summary>
		public Camera Camera { get; private set; }

		/// <summary>
		/// Access to the Integrator settings for this Session
		/// </summary>
		public Integrator Integrator { get; private set; }

		/// <summary>
		/// Access to the Bacground settings for this Session
		/// </summary>
		public Background Background { get; private set; }

		/// <summary>
		/// Access to the Film for this Session
		/// </summary>
		public Film Film { get; private set; }

		/// <summary>
		/// Access to the Device used for this Session
		/// </summary>
		public Device Device { get; private set; }

		/// <summary>
		/// Create a new scene with the given SceneParameters and Device
		/// </summary>
		/// <param name="client">The client from C[CS]ycles API</param>
		/// <param name="sceneParams">The SceneParameters to create scene with</param>
		/// <param name="session">The Session to create scene for</param>
		public Scene(Session session)
		{
			// for now use Session.Id as Session.Id too, since they are now tightly coupled in Cycles
			Id = session.Id;
			Camera = new Camera(session);
			Integrator = new Integrator(session);
			Film = new Film(session);
			Background = new Background(session);
#if SCENESTUFF
// TODO: XXXX scenes are created directly by ccl::Session constructor.
// TODO: XXXX wrap access of scene through session.
			Client = client;
			Id = CSycles.scene_create(sceneParams.Id, session.Id);
			Integrator = new Integrator(this);
			Film = new Film(this);

			/* add simple wrappers for shadermanager created default shaders */
			var surface = Shader.WrapDefaultSurfaceShader(client);
			var light = Shader.WrapDefaultLightShader(client);
			var background = Shader.WrapDefaultBackgroundShader(client);
			var empty = Shader.WrapDefaultEmptyShader(client);

			/* register the wrapped shaders with scene */
			m_shader_in_scene_ids.Add(surface, surface.Id);
			m_shader_in_scene_ids.Add(background, background.Id);
			m_shader_in_scene_ids.Add(light, light.Id);
			m_shader_in_scene_ids.Add(empty, empty.Id);

			DefaultSurface = surface;

			// set ourself to client as reference
			client.Scene = this;
#endif
		}

		/// <summary>
		/// Get the first shader with name
		/// </summary>
		/// <param name="name">Name of shader to look for</param>
		/// <returns>Shader or null if no shader with name was found.</returns>
		public Shader ShaderWithName(string name)
		{
			// TODO: XXXX
			//return (from kvp in m_shader_in_scene_ids where kvp.Key.Name.Equals(name) select kvp.Key).FirstOrDefault();

			return null;
		}

		/// <summary>
		/// Get or set the default surface shader for this scene.
		/// </summary>
		public Shader DefaultSurface
		{
			get
			{
				return new Shader(this, CSycles.scene_get_default_surface_shader(Id));
			}
			set
			{
				if(value != null)
					CSycles.scene_set_default_surface_shader(Id, value.Id);
			}
		}
		//public static uint scene_create(uint scene_params_id, uint deviceid)
		//public static uint scene_add_object(uint scene_id)
		//public static uint scene_add_mesh(uint scene_id, uint object_id, uint shader_id)
		//public static void scene_set_default_surface_shader(uint scene_id, uint shader_id)

		/// <summary>
		/// Reset the scene, forcing update and device update in Cycles.
		/// </summary>
		public void Reset()
		{
			CSycles.scene_reset(Id);
		}

		/// <summary>
		/// Try aqcuire lock on scene mutex non-blocking.
		/// </summary>
		/// <returns>True if lock was acquired, false otherwise</returns>
		public bool TryLock()
		{
			return CSycles.scene_try_lock(Id);
		}

		/// <summary>
		/// Aqcuire lock on scene mutex blocking.
		/// </summary>
		public void Lock()
		{
			CSycles.scene_lock(Id);
		}

		/// <summary>
		/// Release lock.
		/// </summary>
		public void Unlock()
		{
			CSycles.scene_unlock(Id);
		}

		public void ClearClippingPlanes()
		{
			CSycles.scene_clear_clipping_planes(Id);
		}
	}
}
