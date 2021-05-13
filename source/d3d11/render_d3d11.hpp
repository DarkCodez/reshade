/*
 * Copyright (C) 2021 Patrick Mours. All rights reserved.
 * License: https://github.com/crosire/reshade#license
 */

#pragma once

#include "com_ptr.hpp"
#include "com_tracking.hpp"
#include "addon_manager.hpp"
#include <d3d11_4.h>

namespace reshade::d3d11
{
	class device_impl : public api::api_object_impl<ID3D11Device *, api::device>
	{
	public:
		explicit device_impl(ID3D11Device *device);
		~device_impl();

		api::device_api get_api() const final { return api::device_api::d3d11; }

		bool check_capability(api::device_caps capability) const final;
		bool check_format_support(api::format format, api::resource_usage usage) const final;

		bool check_resource_handle_valid(api::resource handle) const final;
		bool check_resource_view_handle_valid(api::resource_view handle) const final;

		bool create_sampler(const api::sampler_desc &desc, api::sampler *out) final;
		bool create_resource(const api::resource_desc &desc, const api::subresource_data *initial_data, api::resource_usage initial_state, api::resource *out) final;
		bool create_resource_view(api::resource resource, api::resource_usage usage_type, const api::resource_view_desc &desc, api::resource_view *out) final;

		bool create_pipeline(const api::pipeline_desc &desc, api::pipeline *out) final;
		bool create_pipeline_compute(const api::pipeline_desc &desc, api::pipeline *out);
		bool create_pipeline_graphics_all(const api::pipeline_desc &desc, api::pipeline *out);
		bool create_pipeline_graphics_blend_state(const api::pipeline_desc &desc, api::pipeline *out);
		bool create_pipeline_graphics_rasterizer_state(const api::pipeline_desc &desc, api::pipeline *out);
		bool create_pipeline_graphics_depth_stencil_state(const api::pipeline_desc &desc, api::pipeline *out);

		bool create_shader_module(api::shader_stage type, api::shader_format format, const char *entry_point, const void *code, size_t code_size, api::shader_module *out) final;
		bool create_pipeline_layout(uint32_t num_table_layouts, const api::descriptor_table_layout *table_layouts, uint32_t num_constant_ranges, const api::constant_range *constant_ranges, api::pipeline_layout *out) final;
		bool create_descriptor_heap(uint32_t max_tables, uint32_t num_sizes, const api::descriptor_heap_size *sizes, api::descriptor_heap *out) final;
		bool create_descriptor_tables(api::descriptor_heap heap, api::descriptor_table_layout layout, uint32_t count, api::descriptor_table *out) final;
		bool create_descriptor_table_layout(uint32_t num_ranges, const api::descriptor_range *ranges, bool push_descriptors, api::descriptor_table_layout *out) final;

		bool create_query_heap(api::query_type type, uint32_t count, api::query_heap *out) final;

		void destroy_sampler(api::sampler handle) final;
		void destroy_resource(api::resource handle) final;
		void destroy_resource_view(api::resource_view handle) final;

		void destroy_pipeline(api::pipeline_type type, api::pipeline handle) final;
		void destroy_shader_module(api::shader_module handle) final;
		void destroy_pipeline_layout(api::pipeline_layout handle) final;
		void destroy_descriptor_heap(api::descriptor_heap handle) final;
		void destroy_descriptor_table_layout(api::descriptor_table_layout handle) final;

		void destroy_query_heap(api::query_heap handle) final;

		void update_descriptor_tables(uint32_t num_updates, const api::descriptor_update *updates) final;

		bool map_resource(api::resource resource, uint32_t subresource, api::map_access access, void **mapped_ptr) final;
		void unmap_resource(api::resource resource, uint32_t subresource) final;

		void upload_buffer_region(const void *data, api::resource dst, uint64_t dst_offset, uint64_t size) final;
		void upload_texture_region(const void *data, uint32_t row_pitch, uint32_t slice_pitch, api::resource dst, uint32_t dst_subresource, const int32_t dst_box[6]) final;

		void get_resource_from_view(api::resource_view view, api::resource *out_resource) const final;
		api::resource_desc get_resource_desc(api::resource resource) const final;

		bool get_query_results(api::query_heap heap, uint32_t first, uint32_t count, void *results, uint32_t stride) final;

		void wait_idle() const final { /* no-op */ }

		void set_debug_name(api::resource resource, const char *name) final;

	protected:
		ID3D11DeviceContext *_immediate_context_orig = nullptr;
		com_object_list<ID3D11View> _views;
		com_object_list<ID3D11Resource> _resources;
	};

	class command_list_impl : public api::api_object_impl<ID3D11CommandList *, api::command_list>
	{
	public:
		command_list_impl(device_impl *device, ID3D11CommandList *cmd_list);
		~command_list_impl();

		api::device *get_device() final { return _device_impl; }

		void bind_pipeline(api::pipeline_type, api::pipeline) final { assert(false); }
		void bind_pipeline_states(uint32_t, const api::pipeline_state *, const uint32_t *) final { assert(false); }
		void bind_viewports(uint32_t, uint32_t, const float *) final { assert(false); }
		void bind_scissor_rects(uint32_t, uint32_t, const int32_t *) final { assert(false); }

		void push_constants(api::shader_stage, api::pipeline_layout, uint32_t, uint32_t, uint32_t, const uint32_t *) final { assert(false); }
		void push_descriptors(api::shader_stage, api::pipeline_layout, uint32_t, api::descriptor_type, uint32_t, uint32_t , const void *) final { assert(false); }
		void bind_descriptor_heaps(uint32_t, const api::descriptor_heap *) final { assert(false); }
		void bind_descriptor_tables(api::pipeline_type, api::pipeline_layout, uint32_t, uint32_t, const api::descriptor_table *) final { assert(false); }

		void bind_index_buffer(api::resource, uint64_t, uint32_t) final { assert(false); }
		void bind_vertex_buffers(uint32_t, uint32_t, const api::resource *, const uint64_t *, const uint32_t *) final { assert(false); }

		void draw(uint32_t, uint32_t, uint32_t, uint32_t) final { assert(false); }
		void draw_indexed(uint32_t, uint32_t, uint32_t, int32_t, uint32_t) final { assert(false); }
		void dispatch(uint32_t, uint32_t, uint32_t) final { assert(false); }
		void draw_or_dispatch_indirect(uint32_t, api::resource, uint64_t, uint32_t, uint32_t) final { assert(false); }

		void begin_render_pass(uint32_t, const api::resource_view *, api::resource_view) final { assert(false); }
		void end_render_pass() final { assert(false); }

		void blit(api::resource, uint32_t, const int32_t[6], api::resource, uint32_t, const int32_t[6], api::texture_filter) final { assert(false); }
		void resolve(api::resource, uint32_t, const int32_t[3], api::resource, uint32_t, const int32_t[3], const uint32_t[3], api::format) final { assert(false); }
		void copy_resource(api::resource, api::resource) final { assert(false); }
		void copy_buffer_region(api::resource, uint64_t, api::resource, uint64_t, uint64_t) final { assert(false); }
		void copy_buffer_to_texture(api::resource, uint64_t, uint32_t, uint32_t, api::resource, uint32_t, const int32_t[6]) final { assert(false); }
		void copy_texture_region(api::resource, uint32_t, const int32_t[3], api::resource, uint32_t, const int32_t[3], const uint32_t[3]) final { assert(false); }
		void copy_texture_to_buffer(api::resource, uint32_t, const int32_t[6], api::resource, uint64_t, uint32_t, uint32_t) final { assert(false); }

		void generate_mipmaps(api::resource_view) final { assert(false); }

		void clear_depth_stencil_view(api::resource_view, uint32_t, float, uint8_t) final { assert(false); }
		void clear_render_target_views(uint32_t, const api::resource_view *, const float[4]) final { assert(false); }
		void clear_unordered_access_view_uint(api::resource_view, const uint32_t[4]) final { assert(false); }
		void clear_unordered_access_view_float(api::resource_view, const float[4]) final { assert(false); }

		void begin_query(api::query_heap, api::query_type, uint32_t) final { assert(false); }
		void end_query(api::query_heap, api::query_type, uint32_t) final { assert(false); }
		void copy_query_results(api::query_heap, api::query_type, uint32_t, uint32_t, api::resource, uint64_t, uint32_t) final { assert(false); }

		void insert_barrier(uint32_t, const api::resource *, const api::resource_usage *, const api::resource_usage *) final { assert(false); }

		void begin_debug_marker(const char *, const float[4]) final { assert(false); }
		void end_debug_marker() final { assert(false); }
		void insert_debug_marker(const char *, const float[4]) final { assert(false); }

	private:
		device_impl *const _device_impl;
	};

	class device_context_impl : public api::api_object_impl<ID3D11DeviceContext *, api::command_list, api::command_queue>
	{
	public:
		device_context_impl(device_impl *device, ID3D11DeviceContext *context);
		~device_context_impl();

		api::device *get_device() final { return _device_impl; }

		api::command_list *get_immediate_command_list() final { assert(_orig->GetType() == D3D11_DEVICE_CONTEXT_IMMEDIATE); return this; }

		void flush_immediate_command_list() const final;

		void wait_idle() const final { /* no-op */ }

		void bind_pipeline(api::pipeline_type type, api::pipeline pipeline) final;
		void bind_pipeline_states(uint32_t count, const api::pipeline_state *states, const uint32_t *values) final;
		void bind_viewports(uint32_t first, uint32_t count, const float *viewports) final;
		void bind_scissor_rects(uint32_t first, uint32_t count, const int32_t *rects) final;

		void bind_samplers(api::shader_stage stage, uint32_t first, uint32_t count, const api::sampler *samplers);
		void bind_shader_resource_views(api::shader_stage stage, uint32_t first, uint32_t count, const api::resource_view *views);
		void bind_unordered_access_views(api::shader_stage stage, uint32_t first, uint32_t count, const api::resource_view *views);
		void bind_constant_buffers(api::shader_stage stage, uint32_t first, uint32_t count, const api::resource *buffers);

		void push_constants(api::shader_stage stage, api::pipeline_layout layout, uint32_t layout_index, uint32_t first, uint32_t count, const uint32_t *values) final;
		void push_descriptors(api::shader_stage stage, api::pipeline_layout layout, uint32_t layout_index, api::descriptor_type type, uint32_t first, uint32_t count, const void *descriptors) final;
		void bind_descriptor_heaps(uint32_t count, const api::descriptor_heap *heaps) final;
		void bind_descriptor_tables(api::pipeline_type type, api::pipeline_layout layout, uint32_t first, uint32_t count, const api::descriptor_table *tables) final;

		void bind_index_buffer(api::resource buffer, uint64_t offset, uint32_t index_size) final;
		void bind_vertex_buffers(uint32_t first, uint32_t count, const api::resource *buffers, const uint64_t *offsets, const uint32_t *strides) final;

		void draw(uint32_t vertices, uint32_t instances, uint32_t first_vertex, uint32_t first_instance) final;
		void draw_indexed(uint32_t indices, uint32_t instances, uint32_t first_index, int32_t vertex_offset, uint32_t first_instance) final;
		void dispatch(uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z) final;
		void draw_or_dispatch_indirect(uint32_t type, api::resource buffer, uint64_t offset, uint32_t draw_count, uint32_t stride) final;

		void begin_render_pass(uint32_t count, const api::resource_view *rtvs, api::resource_view dsv) final;
		void end_render_pass() final;

		void blit(api::resource src, uint32_t src_subresource, const int32_t src_box[6], api::resource dst, uint32_t dst_subresource, const int32_t dst_box[6], api::texture_filter filter) final;
		void resolve(api::resource src, uint32_t src_subresource, const int32_t src_offset[3], api::resource dst, uint32_t dst_subresource, const int32_t dst_offset[3], const uint32_t size[3], api::format format) final;
		void copy_resource(api::resource src, api::resource dst) final;
		void copy_buffer_region(api::resource src, uint64_t src_offset, api::resource dst, uint64_t dst_offset, uint64_t size) final;
		void copy_buffer_to_texture(api::resource src, uint64_t src_offset, uint32_t row_length, uint32_t slice_height, api::resource dst, uint32_t dst_subresource, const int32_t dst_box[6]) final;
		void copy_texture_region(api::resource src, uint32_t src_subresource, const int32_t src_offset[3], api::resource dst, uint32_t dst_subresource, const int32_t dst_offset[3], const uint32_t size[3]) final;
		void copy_texture_to_buffer(api::resource src, uint32_t src_subresource, const int32_t src_box[6], api::resource dst, uint64_t dst_offset, uint32_t row_length, uint32_t slice_height) final;

		void generate_mipmaps(api::resource_view srv) final;

		void clear_depth_stencil_view(api::resource_view dsv, uint32_t clear_flags, float depth, uint8_t stencil) final;
		void clear_render_target_views(uint32_t count, const api::resource_view *rtvs, const float color[4]) final;
		void clear_unordered_access_view_uint(api::resource_view uav, const uint32_t values[4]) final;
		void clear_unordered_access_view_float(api::resource_view uav, const float values[4]) final;

		void begin_query(api::query_heap heap, api::query_type type, uint32_t index) final;
		void end_query(api::query_heap heap, api::query_type type, uint32_t index) final;
		void copy_query_results(api::query_heap heap, api::query_type type, uint32_t first, uint32_t count, api::resource dst, uint64_t dst_offset, uint32_t stride) final;

		void insert_barrier(uint32_t, const api::resource *, const api::resource_usage *, const api::resource_usage *) final { /* no-op */ }

		void begin_debug_marker(const char *label, const float color[4]) final;
		void end_debug_marker() final;
		void insert_debug_marker(const char *label, const float color[4]) final;

	protected:
		bool _has_open_render_pass = false;

	private:
		device_impl *const _device_impl;
		com_ptr<ID3DUserDefinedAnnotation> _annotations;
		UINT _push_constants_size = 0;
		com_ptr<ID3D11Buffer> _push_constants;
	};
}
