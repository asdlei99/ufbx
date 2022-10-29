
UFBXT_FILE_TEST(zbrush_vertex_color)
#if UFBXT_IMPL
{
	ufbxt_assert(scene->nodes.count == 2);
	ufbx_node *node = scene->nodes.data[1];
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->vertex_color.exists);
	ufbxt_assert(mesh->vertex_color.unique_per_vertex);

	ufbxt_assert(mesh->num_vertices == 6);
	for (size_t i = 0; i < mesh->num_vertices; i++) {
		ufbx_vec3 pos = mesh->vertex_position.values.data[i];
		ufbx_vec4 color = ufbx_get_vertex_vec4(&mesh->vertex_color, mesh->vertex_first_index.data[i]);
		ufbx_vec4 ref = { 0.0f, 0.0f, 0.0f, 1.0f };

		pos.y -= 1.0f;

		if (pos.x < -0.5f) {
			ref.x = 1.0f;
		} else if (pos.x > 0.5f) {
			ref.y = ref.z = 1.0f;
		} else if (pos.y > 0.5f) {
			ref.y = 1.0f;
		} else if (pos.y < -0.5f) {
			ref.x = ref.z = 1.0f;
		} else if (pos.z > 0.5f) {
			ref.z = 1.0f;
		} else if (pos.z < -0.5f) {
			ref.x = ref.y = 1.0f;
		}

		ufbxt_assert_close_vec4(err, color, ref);
		ufbxt_assert_close_vec4(err, color, ref);
	}

}
#endif

UFBXT_FILE_TEST(synthetic_color_suzanne)
#if UFBXT_IMPL
{
	ufbxt_assert(scene->nodes.count == 2);
	ufbx_node *node = scene->nodes.data[1];
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->vertex_color.exists);
	ufbxt_assert(mesh->vertex_color.unique_per_vertex);

	ufbxt_assert(mesh->num_faces == 500);
	ufbxt_assert(mesh->num_triangles == 968);

	for (size_t i = 0; i < mesh->num_indices; i++) {
		ufbx_vec3 position = ufbx_get_vertex_vec3(&mesh->vertex_position, i);
		ufbx_vec4 color = ufbx_get_vertex_vec4(&mesh->vertex_color, i);

		ufbx_vec3 col = { color.x, color.y, color.z };

		ufbx_vec3 ref;
		ref.x = ufbxt_clamp(position.x * 0.5f + 0.5f, 0.0f, 1.0f);
		ref.y = ufbxt_clamp(position.y * 0.5f + 0.5f, 0.0f, 1.0f);
		ref.z = ufbxt_clamp(position.z * 0.5f + 0.5f, 0.0f, 1.0f);

		ufbxt_assert_close_vec3_threshold(err, col, ref, (ufbx_real)(1.0/256.0));
	}
}
#endif

#if UFBXT_IMPL
static void ufbxt_check_obj_elements(ufbxt_diff_error *err, ufbx_scene *scene, int32_t v, int32_t vt, int32_t vn, int32_t vc, const char *name)
{
	ufbxt_hintf("name = \"%s\"", name);

	ufbx_node *node = ufbx_find_node(scene, name);
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;
	ufbxt_assert(!strcmp(mesh->name.data, name));

	ufbxt_assert(mesh->num_faces == 1);
	ufbxt_assert(mesh->num_triangles == 1);

	ufbx_face face = mesh->faces.data[0];
	ufbxt_assert(face.index_begin == 0);
	ufbxt_assert(face.num_indices == 3);

	if (v > 0) {
		ufbxt_assert(mesh->vertex_position.exists);
		ufbxt_assert(mesh->vertex_position.indices.count == 3);
		const ufbx_vec3 refs[] = {
			{ (ufbx_real)-v, 0.0f, (ufbx_real)(v - 1) },
			{ (ufbx_real)+v, 0.0f, (ufbx_real)(v - 1) },
			{ 0.0f, (ufbx_real)+v, (ufbx_real)(v - 1) },
		};
		for (size_t ix = 0; ix < 3; ix++) {
			ufbx_vec3 val = ufbx_get_vertex_vec3(&mesh->vertex_position, ix);
			ufbxt_assert_close_vec3(err, val, refs[ix]);
		}
	} else {
		ufbxt_assert(!mesh->vertex_position.exists);
	}

	if (vt > 0) {
		ufbxt_assert(mesh->vertex_uv.exists);
		ufbxt_assert(mesh->vertex_uv.indices.count == 3);
		const ufbx_vec2 refs[] = {
			{ 0.0f, 0.0f },
			{ (ufbx_real)vt, 0.0f },
			{ 0.0f, (ufbx_real)vt },
		};
		for (size_t ix = 0; ix < 3; ix++) {
			ufbx_vec2 val = ufbx_get_vertex_vec2(&mesh->vertex_uv, ix);
			ufbxt_assert_close_vec2(err, val, refs[ix]);
		}
	} else {
		ufbxt_assert(!mesh->vertex_uv.exists);
	}

	if (vn > 0) {
		ufbxt_assert(mesh->vertex_normal.exists);
		ufbxt_assert(mesh->vertex_normal.indices.count == 3);
		const ufbx_vec3 refs[] = {
			{ 0.0f, (ufbx_real)-vn, 0.0f },
			{ 0.0f, (ufbx_real)-vn, 0.0f },
			{ 0.0f, (ufbx_real)+vn, 0.0f },
		};
		for (size_t ix = 0; ix < 3; ix++) {
			ufbx_vec3 val = ufbx_get_vertex_vec3(&mesh->vertex_normal, ix);
			ufbxt_assert_close_vec3(err, val, refs[ix]);
		}
	} else {
		ufbxt_assert(!mesh->vertex_normal.exists);
	}

	if (vc > 0) {
		ufbxt_assert(mesh->vertex_color.exists);
		ufbxt_assert(mesh->vertex_color.indices.count == 3);
		const ufbx_vec4 refs[] = {
			{ (ufbx_real)vc, 0.0f, 0.0f, 1.0f },
			{ 0.0f, (ufbx_real)vc, 0.0f, 1.0f },
			{ 0.0f, 0.0f, (ufbx_real)vc, 1.0f },
		};
		for (size_t ix = 0; ix < 3; ix++) {
			ufbx_vec4 val = ufbx_get_vertex_vec4(&mesh->vertex_color, ix);
			ufbxt_assert_close_vec4(err, val, refs[ix]);
		}
	} else {
		ufbxt_assert(!mesh->vertex_color.exists);
	}
}
#endif

UFBXT_FILE_TEST(synthetic_mixed_attribs)
#if UFBXT_IMPL
{
	ufbxt_assert(scene->nodes.count == 9);
	ufbxt_assert(scene->meshes.count == 8);
	ufbxt_check_obj_elements(err, scene, 1, 0, 0, 0, "V");
	ufbxt_check_obj_elements(err, scene, 2, 1, 0, 0, "VT");
	ufbxt_check_obj_elements(err, scene, 3, 0, 1, 0, "VN");
	ufbxt_check_obj_elements(err, scene, 4, 2, 2, 0, "VTN");
	ufbxt_check_obj_elements(err, scene, 5, 0, 0, 1, "VC");
	ufbxt_check_obj_elements(err, scene, 6, 3, 0, 2, "VTC");
	ufbxt_check_obj_elements(err, scene, 7, 0, 3, 3, "VNC");
	ufbxt_check_obj_elements(err, scene, 8, 4, 4, 4, "VTNC");
}
#endif

UFBXT_FILE_TEST(synthetic_mixed_attribs_reverse)
#if UFBXT_IMPL
{
	ufbxt_assert(scene->nodes.count == 9);
	ufbxt_assert(scene->meshes.count == 8);
	ufbxt_check_obj_elements(err, scene, 1, 0, 0, 0, "V");
	ufbxt_check_obj_elements(err, scene, 2, 1, 0, 0, "VT");
	ufbxt_check_obj_elements(err, scene, 3, 0, 1, 0, "VN");
	ufbxt_check_obj_elements(err, scene, 4, 2, 2, 0, "VTN");
	ufbxt_check_obj_elements(err, scene, 5, 0, 0, 1, "VC");
	ufbxt_check_obj_elements(err, scene, 6, 3, 0, 2, "VTC");
	ufbxt_check_obj_elements(err, scene, 7, 0, 3, 3, "VNC");
	ufbxt_check_obj_elements(err, scene, 8, 4, 4, 4, "VTNC");
}
#endif

UFBXT_FILE_TEST(synthetic_mixed_attribs_reuse)
#if UFBXT_IMPL
{
	ufbxt_assert(scene->nodes.count == 9);
	ufbxt_assert(scene->meshes.count == 8);
	ufbxt_check_obj_elements(err, scene, 1, 0, 0, 0, "V");
	ufbxt_check_obj_elements(err, scene, 1, 1, 0, 0, "VT");
	ufbxt_check_obj_elements(err, scene, 1, 0, 1, 0, "VN");
	ufbxt_check_obj_elements(err, scene, 1, 1, 1, 0, "VTN");
	ufbxt_check_obj_elements(err, scene, 2, 0, 0, 1, "VC");
	ufbxt_check_obj_elements(err, scene, 2, 1, 0, 1, "VTC");
	ufbxt_check_obj_elements(err, scene, 2, 0, 1, 1, "VNC");
	ufbxt_check_obj_elements(err, scene, 2, 1, 1, 1, "VTNC");
}
#endif

#if UFBXT_IMPL
static ufbx_load_opts ufbxt_no_index_opts()
{
	ufbx_load_opts opts = { 0 };
	opts.index_error_handling = UFBX_INDEX_ERROR_HANDLING_NO_INDEX;
	return opts;
}

static ufbx_load_opts ufbxt_abort_index_opts()
{
	ufbx_load_opts opts = { 0 };
	opts.index_error_handling = UFBX_INDEX_ERROR_HANDLING_ABORT_LOADING;
	return opts;
}

static void ufbxt_check_obj_face(ufbx_mesh *mesh, size_t face_ix, int32_t v, int32_t vt, int32_t vn, int32_t vc, bool no_index)
{
	ufbxt_hintf("face_ix = %zu", face_ix);

	ufbxt_assert(face_ix < mesh->faces.count);
	ufbx_face face = mesh->faces.data[face_ix];
	uint32_t a = face.index_begin + 0;
	uint32_t b = face.index_begin + 1;
	uint32_t c = face.index_begin + 2;

	if (v) {
		ufbxt_assert(mesh->vertex_position.indices.data[a] == v - 1 + 0);
		ufbxt_assert(mesh->vertex_position.indices.data[b] == v - 1 + 1);
		ufbxt_assert(mesh->vertex_position.indices.data[c] == v - 1 + 2);
	} else {
		uint32_t sentinel = no_index ? UFBX_NO_INDEX : (uint32_t)mesh->vertex_position.values.count - 1;
		ufbxt_assert(mesh->vertex_position.indices.data[a] == sentinel);
		ufbxt_assert(mesh->vertex_position.indices.data[b] == sentinel);
		ufbxt_assert(mesh->vertex_position.indices.data[c] == sentinel);
	}

	if (vt) {
		ufbxt_assert(mesh->vertex_uv.indices.data[a] == vt - 1 + 0);
		ufbxt_assert(mesh->vertex_uv.indices.data[b] == vt - 1 + 1);
		ufbxt_assert(mesh->vertex_uv.indices.data[c] == vt - 1 + 2);
	} else {
		uint32_t sentinel = no_index ? UFBX_NO_INDEX : (uint32_t)mesh->vertex_uv.values.count - 1;
		ufbxt_assert(mesh->vertex_uv.indices.data[a] == sentinel);
		ufbxt_assert(mesh->vertex_uv.indices.data[b] == sentinel);
		ufbxt_assert(mesh->vertex_uv.indices.data[c] == sentinel);
	}

	if (vn) {
		ufbxt_assert(mesh->vertex_normal.indices.data[a] == vn - 1 + 0);
		ufbxt_assert(mesh->vertex_normal.indices.data[b] == vn - 1 + 1);
		ufbxt_assert(mesh->vertex_normal.indices.data[c] == vn - 1 + 2);
	} else {
		uint32_t sentinel = no_index ? UFBX_NO_INDEX : (uint32_t)mesh->vertex_normal.values.count - 1;
		ufbxt_assert(mesh->vertex_normal.indices.data[a] == sentinel);
		ufbxt_assert(mesh->vertex_normal.indices.data[b] == sentinel);
		ufbxt_assert(mesh->vertex_normal.indices.data[c] == sentinel);
	}

	if (vc) {
		ufbxt_assert(mesh->vertex_color.indices.data[a] == vc - 1 + 0);
		ufbxt_assert(mesh->vertex_color.indices.data[b] == vc - 1 + 1);
		ufbxt_assert(mesh->vertex_color.indices.data[c] == vc - 1 + 2);
	} else {
		uint32_t sentinel = no_index ? UFBX_NO_INDEX : (uint32_t)mesh->vertex_color.values.count - 1;
		ufbxt_assert(mesh->vertex_color.indices.data[a] == sentinel);
		ufbxt_assert(mesh->vertex_color.indices.data[b] == sentinel);
		ufbxt_assert(mesh->vertex_color.indices.data[c] == sentinel);
	}
}

static void ufbxt_check_obj_index(ufbx_mesh *mesh, size_t index, int32_t v, int32_t vt, int32_t vn, int32_t vc, bool no_index)
{
	ufbxt_hintf("index = %zu", index);
	ufbxt_assert(index < mesh->num_indices);

	if (v) {
		ufbxt_assert(mesh->vertex_position.indices.data[index] == v - 1);
	} else {
		uint32_t sentinel = no_index ? UFBX_NO_INDEX : (uint32_t)mesh->vertex_position.values.count - 1;
		ufbxt_assert(mesh->vertex_position.indices.data[index] == sentinel);
	}

	if (vt) {
		ufbxt_assert(mesh->vertex_uv.indices.data[index] == vt - 1);
	} else {
		uint32_t sentinel = no_index ? UFBX_NO_INDEX : (uint32_t)mesh->vertex_uv.values.count - 1;
		ufbxt_assert(mesh->vertex_uv.indices.data[index] == sentinel);
	}

	if (vn) {
		ufbxt_assert(mesh->vertex_normal.indices.data[index] == vn - 1);
	} else {
		uint32_t sentinel = no_index ? UFBX_NO_INDEX : (uint32_t)mesh->vertex_normal.values.count - 1;
		ufbxt_assert(mesh->vertex_normal.indices.data[index] == sentinel);
	}

	if (vc) {
		ufbxt_assert(mesh->vertex_color.indices.data[index] == vc - 1);
	} else {
		uint32_t sentinel = no_index ? UFBX_NO_INDEX : (uint32_t)mesh->vertex_color.values.count - 1;
		ufbxt_assert(mesh->vertex_color.indices.data[index] == sentinel);
	}
}

#endif

UFBXT_FILE_TEST(synthetic_partial_attrib)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "Mesh");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->num_faces == 12);
	ufbxt_assert(mesh->num_triangles == 12);
	ufbxt_assert(mesh->vertex_position.exists);
	ufbxt_assert(mesh->vertex_uv.exists);
	ufbxt_assert(mesh->vertex_normal.exists);
	ufbxt_assert(mesh->vertex_color.exists);

	ufbxt_check_obj_face(mesh,  0, 1, 0, 0, 0, false);
	ufbxt_check_obj_face(mesh,  1, 1, 1, 0, 0, false);
	ufbxt_check_obj_face(mesh,  2, 1, 0, 1, 0, false);
	ufbxt_check_obj_face(mesh,  3, 1, 1, 1, 0, false);
	ufbxt_check_obj_face(mesh,  4, 4, 0, 0, 4, false);
	ufbxt_check_obj_face(mesh,  5, 4, 1, 0, 4, false);
	ufbxt_check_obj_face(mesh,  6, 4, 0, 1, 4, false);
	ufbxt_check_obj_face(mesh,  7, 4, 1, 1, 4, false);
	ufbxt_check_obj_face(mesh,  8, 0, 0, 0, 0, false);
	ufbxt_check_obj_face(mesh,  9, 0, 1, 0, 0, false);
	ufbxt_check_obj_face(mesh, 10, 0, 0, 1, 0, false);
	ufbxt_check_obj_face(mesh, 11, 0, 1, 1, 0, false);
}
#endif

UFBXT_FILE_TEST_OPTS_ALT(synthetic_partial_attrib_no_index, synthetic_partial_attrib, ufbxt_no_index_opts)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "Mesh");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->num_faces == 12);
	ufbxt_assert(mesh->num_triangles == 12);
	ufbxt_assert(mesh->vertex_position.exists);
	ufbxt_assert(mesh->vertex_uv.exists);
	ufbxt_assert(mesh->vertex_normal.exists);
	ufbxt_assert(mesh->vertex_color.exists);

	ufbxt_check_obj_face(mesh,  0, 1, 0, 0, 0, true);
	ufbxt_check_obj_face(mesh,  1, 1, 1, 0, 0, true);
	ufbxt_check_obj_face(mesh,  2, 1, 0, 1, 0, true);
	ufbxt_check_obj_face(mesh,  3, 1, 1, 1, 0, true);
	ufbxt_check_obj_face(mesh,  4, 4, 0, 0, 4, true);
	ufbxt_check_obj_face(mesh,  5, 4, 1, 0, 4, true);
	ufbxt_check_obj_face(mesh,  6, 4, 0, 1, 4, true);
	ufbxt_check_obj_face(mesh,  7, 4, 1, 1, 4, true);
	ufbxt_check_obj_face(mesh,  8, 0, 0, 0, 0, true);
	ufbxt_check_obj_face(mesh,  9, 0, 1, 0, 0, true);
	ufbxt_check_obj_face(mesh, 10, 0, 0, 1, 0, true);
	ufbxt_check_obj_face(mesh, 11, 0, 1, 1, 0, true);
}
#endif

UFBXT_FILE_TEST_OPTS_ALT_FLAGS(synthetic_partial_attrib_strict, synthetic_partial_attrib, ufbxt_abort_index_opts, UFBXT_FILE_TEST_FLAG_ALLOW_ERROR)
#if UFBXT_IMPL
{
	ufbxt_assert(!scene);
	ufbxt_assert(load_error);
	ufbxt_assert(load_error->type == UFBX_ERROR_BAD_INDEX);
}
#endif

UFBXT_FILE_TEST(synthetic_partial_attrib_face)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "Mesh");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->num_faces == 1);
	ufbxt_assert(mesh->num_triangles == 10);
	ufbxt_assert(mesh->num_indices == 12);
	ufbxt_assert(mesh->vertex_position.exists);
	ufbxt_assert(mesh->vertex_uv.exists);
	ufbxt_assert(mesh->vertex_normal.exists);
	ufbxt_assert(mesh->vertex_color.exists);

	ufbx_face face = mesh->faces.data[0];
	ufbxt_assert(face.index_begin == 0);
	ufbxt_assert(face.num_indices == 12);

	ufbxt_check_obj_index(mesh,  0, 1, 0, 0, 0, false);
	ufbxt_check_obj_index(mesh,  1, 2, 1, 0, 0, false);
	ufbxt_check_obj_index(mesh,  2, 3, 0, 1, 0, false);
	ufbxt_check_obj_index(mesh,  3, 4, 2, 2, 0, false);
	ufbxt_check_obj_index(mesh,  4, 5, 0, 0, 5, false);
	ufbxt_check_obj_index(mesh,  5, 6, 3, 0, 6, false);
	ufbxt_check_obj_index(mesh,  6, 7, 0, 3, 7, false);
	ufbxt_check_obj_index(mesh,  7, 8, 4, 4, 8, false);
	ufbxt_check_obj_index(mesh,  8, 0, 0, 0, 0, false);
	ufbxt_check_obj_index(mesh,  9, 0, 5, 0, 0, false);
	ufbxt_check_obj_index(mesh, 10, 0, 0, 5, 0, false);
	ufbxt_check_obj_index(mesh, 11, 0, 6, 6, 0, false);
}
#endif

UFBXT_FILE_TEST_OPTS_ALT(synthetic_partial_attrib_face_no_index, synthetic_partial_attrib_face, ufbxt_no_index_opts)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "Mesh");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->num_faces == 1);
	ufbxt_assert(mesh->num_triangles == 10);
	ufbxt_assert(mesh->num_indices == 12);
	ufbxt_assert(mesh->vertex_position.exists);
	ufbxt_assert(mesh->vertex_uv.exists);
	ufbxt_assert(mesh->vertex_normal.exists);
	ufbxt_assert(mesh->vertex_color.exists);

	ufbx_face face = mesh->faces.data[0];
	ufbxt_assert(face.index_begin == 0);
	ufbxt_assert(face.num_indices == 12);

	ufbxt_check_obj_index(mesh,  0, 1, 0, 0, 0, true);
	ufbxt_check_obj_index(mesh,  1, 2, 1, 0, 0, true);
	ufbxt_check_obj_index(mesh,  2, 3, 0, 1, 0, true);
	ufbxt_check_obj_index(mesh,  3, 4, 2, 2, 0, true);
	ufbxt_check_obj_index(mesh,  4, 5, 0, 0, 5, true);
	ufbxt_check_obj_index(mesh,  5, 6, 3, 0, 6, true);
	ufbxt_check_obj_index(mesh,  6, 7, 0, 3, 7, true);
	ufbxt_check_obj_index(mesh,  7, 8, 4, 4, 8, true);
	ufbxt_check_obj_index(mesh,  8, 0, 0, 0, 0, true);
	ufbxt_check_obj_index(mesh,  9, 0, 5, 0, 0, true);
	ufbxt_check_obj_index(mesh, 10, 0, 0, 5, 0, true);
	ufbxt_check_obj_index(mesh, 11, 0, 6, 6, 0, true);
}
#endif

UFBXT_FILE_TEST_OPTS_ALT_FLAGS(synthetic_partial_attrib_face_strict, synthetic_partial_attrib_face, ufbxt_abort_index_opts, UFBXT_FILE_TEST_FLAG_ALLOW_ERROR)
#if UFBXT_IMPL
{
	ufbxt_assert(!scene);
	ufbxt_assert(load_error);
	ufbxt_assert(load_error->type == UFBX_ERROR_BAD_INDEX);
}
#endif

UFBXT_FILE_TEST(synthetic_simple_materials)
#if UFBXT_IMPL
{
	ufbxt_assert(scene->materials.count == 3);

	{
		ufbx_material *mat = ufbx_find_material(scene, "RGB");
		ufbxt_assert(mat);
		ufbxt_assert(mat->shader_type == UFBX_SHADER_WAVEFRONT_MTL);

		ufbx_vec3 ka = { 1.0f, 0.0f, 0.0f };
		ufbx_vec3 kd = { 0.0f, 1.0f, 0.0f };
		ufbx_vec3 ks = { 0.0f, 0.0f, 1.0f };
		ufbx_vec3 ke = { 1.0f, 0.0f, 1.0f };
		ufbx_real ns = 99.0f;
		ufbx_real d = 0.25f;

		ufbxt_assert_close_vec3(err, mat->fbx.ambient_color.value_vec3, ka);
		ufbxt_assert_close_vec3(err, mat->fbx.diffuse_color.value_vec3, kd);
		ufbxt_assert_close_vec3(err, mat->fbx.specular_color.value_vec3, ks);
		ufbxt_assert_close_vec3(err, mat->fbx.emission_color.value_vec3, ke);
		ufbxt_assert_close_real(err, mat->fbx.specular_exponent.value_real, ns);
		ufbxt_assert_close_real(err, mat->fbx.transparency_factor.value_real, d);
		ufbxt_assert(mat->fbx.ambient_factor.value_real == 1.0f);
		ufbxt_assert(mat->fbx.diffuse_factor.value_real == 1.0f);
		ufbxt_assert(mat->fbx.specular_factor.value_real == 1.0f);
		ufbxt_assert(mat->fbx.emission_factor.value_real == 1.0f);

		ufbxt_assert_close_vec3(err, mat->pbr.base_color.value_vec3, kd);
		ufbxt_assert_close_vec3(err, mat->pbr.specular_color.value_vec3, ks);
		ufbxt_assert_close_vec3(err, mat->pbr.emission_color.value_vec3, ke);
		ufbxt_assert_close_real(err, mat->pbr.roughness.value_real, 0.00501256289f);
		ufbxt_assert_close_real(err, mat->pbr.opacity.value_real, d);
		ufbxt_assert(mat->pbr.base_factor.value_real == 1.0f);
		ufbxt_assert(mat->pbr.specular_factor.value_real == 1.0f);
		ufbxt_assert(mat->pbr.emission_factor.value_real == 1.0f);
	}

	{
		ufbx_material *mat = ufbx_find_material(scene, "PBR");
		ufbxt_assert(mat);
		ufbxt_assert(mat->shader_type == UFBX_SHADER_WAVEFRONT_MTL);

		ufbx_real pr = 0.1f;
		ufbx_real pm = 0.2f;
		ufbx_vec3 ps = { 0.3f, 0.4f, 0.5f };
		ufbx_real pc = 0.6f;
		ufbx_real pcr = 0.7f;
		ufbx_real ni = 1.33f;
		ufbx_vec3 tf = { 0.8f, 0.9f, 1.0f };
		ufbx_real d = 0.75f;

		ufbxt_assert_close_real(err, mat->pbr.roughness.value_real, pr);
		ufbxt_assert_close_real(err, mat->pbr.metalness.value_real, pm);
		ufbxt_assert_close_vec3(err, mat->pbr.sheen_color.value_vec3, ps);
		ufbxt_assert_close_real(err, mat->pbr.coat_factor.value_real, pc);
		ufbxt_assert_close_real(err, mat->pbr.coat_roughness.value_real, pcr);
		ufbxt_assert_close_real(err, mat->pbr.specular_ior.value_real, ni);
		ufbxt_assert_close_vec3(err, mat->pbr.transmission_color.value_vec3, tf);
		ufbxt_assert_close_real(err, mat->pbr.opacity.value_real, d);

		ufbxt_assert(mat->pbr.sheen_factor.value_real == 1.0f);
		ufbxt_assert(mat->pbr.transmission_factor.value_real == 1.0f);

		ufbxt_assert(mat->features.metalness.enabled);
		ufbxt_assert(mat->features.diffuse.enabled);
		ufbxt_assert(mat->features.specular.enabled);
		ufbxt_assert(mat->features.sheen.enabled);
		ufbxt_assert(mat->features.coat.enabled);
		ufbxt_assert(mat->features.transmission.enabled);
		ufbxt_assert(mat->features.opacity.enabled);
	}

	{
		ufbx_material *mat = ufbx_find_material(scene, "Wide");
		ufbxt_assert(mat);
		ufbxt_assert(mat->shader_type == UFBX_SHADER_WAVEFRONT_MTL);

		ufbx_vec3 ka = { 0.1f, 0.1f, 0.1f };
		ufbx_vec3 kd = { 0.2f, 0.2f, 0.2f };
		ufbx_vec3 ks = { 0.3f, 0.3f, 0.3f };
		ufbx_vec3 ke = { 0.4f, 0.4f, 0.4f };
		ufbx_vec3 ps = { 0.5f, 0.5f, 0.5f };
		ufbx_vec3 tf = { 0.6f, 0.6f, 0.6f };

		ufbxt_assert_close_vec3(err, mat->fbx.ambient_color.value_vec3, ka);
		ufbxt_assert_close_vec3(err, mat->fbx.diffuse_color.value_vec3, kd);
		ufbxt_assert_close_vec3(err, mat->fbx.specular_color.value_vec3, ks);
		ufbxt_assert_close_vec3(err, mat->fbx.emission_color.value_vec3, ke);

		ufbxt_assert_close_vec3(err, mat->pbr.base_color.value_vec3, kd);
		ufbxt_assert_close_vec3(err, mat->pbr.specular_color.value_vec3, ks);
		ufbxt_assert_close_vec3(err, mat->pbr.emission_color.value_vec3, ke);
		ufbxt_assert_close_vec3(err, mat->pbr.sheen_color.value_vec3, ps);
		ufbxt_assert_close_vec3(err, mat->pbr.transmission_color.value_vec3, tf);

	}
}
#endif

#if UFBXT_IMPL
void ufbxt_check_obj_texture(ufbx_scene *scene, ufbx_texture *texture, const char *filename)
{
	char rel_path[256];
	snprintf(rel_path, sizeof(rel_path), "textures/%s", filename);

	ufbxt_assert(texture);
	ufbxt_assert(!strcmp(texture->relative_filename.data, rel_path));
}
#endif

UFBXT_FILE_TEST(synthetic_simple_textures)
#if UFBXT_IMPL
{
	ufbxt_assert(scene->materials.count == 2);

	{
		ufbx_material *mat = ufbx_find_material(scene, "RGB");
		ufbxt_assert(mat);
		ufbxt_assert(mat->shader_type == UFBX_SHADER_WAVEFRONT_MTL);

		ufbxt_check_obj_texture(scene, mat->fbx.ambient_color.texture, "checkerboard_ambient.png");
		ufbxt_check_obj_texture(scene, mat->fbx.diffuse_color.texture, "checkerboard_diffuse.png");
		ufbxt_check_obj_texture(scene, mat->fbx.specular_color.texture, "checkerboard_specular.png");
		ufbxt_check_obj_texture(scene, mat->fbx.emission_color.texture, "checkerboard_emissive.png");
		ufbxt_check_obj_texture(scene, mat->fbx.specular_exponent.texture, "checkerboard_roughness.png");
		ufbxt_check_obj_texture(scene, mat->fbx.transparency_factor.texture, "checkerboard_transparency.png");

		ufbxt_check_obj_texture(scene, mat->pbr.base_color.texture, "checkerboard_diffuse.png");
		ufbxt_check_obj_texture(scene, mat->pbr.specular_color.texture, "checkerboard_specular.png");
		ufbxt_check_obj_texture(scene, mat->pbr.emission_color.texture, "checkerboard_emissive.png");
		ufbxt_check_obj_texture(scene, mat->pbr.roughness.texture, "checkerboard_roughness.png");
	}

	{
		ufbx_material *mat = ufbx_find_material(scene, "PBR");
		ufbxt_assert(mat);
		ufbxt_assert(mat->shader_type == UFBX_SHADER_WAVEFRONT_MTL);

		ufbxt_check_obj_texture(scene, mat->pbr.roughness.texture, "checkerboard_roughness.png");
		ufbxt_check_obj_texture(scene, mat->pbr.metalness.texture, "checkerboard_metallic.png");
		ufbxt_check_obj_texture(scene, mat->pbr.sheen_color.texture, "checkerboard_reflection.png");
		ufbxt_check_obj_texture(scene, mat->pbr.coat_factor.texture, "checkerboard_specular.png");
		ufbxt_check_obj_texture(scene, mat->pbr.coat_roughness.texture, "checkerboard_weight.png");
		ufbxt_check_obj_texture(scene, mat->pbr.transmission_color.texture, "checkerboard_transparency.png");
		ufbxt_check_obj_texture(scene, mat->pbr.opacity.texture, "checkerboard_weight.png");
		ufbxt_check_obj_texture(scene, mat->pbr.specular_ior.texture, "checkerboard_specular.png");
		ufbxt_check_obj_texture(scene, mat->pbr.normal_map.texture, "checkerboard_normal.png");
		ufbxt_check_obj_texture(scene, mat->pbr.displacement_map.texture, "checkerboard_displacement.png");

		ufbxt_check_obj_texture(scene, mat->fbx.transparency_factor.texture, "checkerboard_weight.png");
		ufbxt_check_obj_texture(scene, mat->fbx.normal_map.texture, "checkerboard_normal.png");
		ufbxt_check_obj_texture(scene, mat->fbx.displacement.texture, "checkerboard_displacement.png");
	}

}
#endif
