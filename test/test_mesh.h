
UFBXT_FILE_TEST(blender_279_default)
#if UFBXT_IMPL
{
	if (scene->metadata.ascii) {
		ufbxt_assert(scene->metadata.exporter == UFBX_EXPORTER_BLENDER_ASCII);
		ufbxt_assert(scene->metadata.exporter_version == ufbx_pack_version(2, 79, 0));
	} else {
		ufbxt_assert(scene->metadata.exporter == UFBX_EXPORTER_BLENDER_BINARY);
		ufbxt_assert(scene->metadata.exporter_version == ufbx_pack_version(3, 7, 13));
	}

	ufbx_node *node = ufbx_find_node(scene, "Lamp");
	ufbxt_assert(node);
	ufbx_light *light = node->light;
	ufbxt_assert(light);

	// Light attribute properties
	ufbx_vec3 color_ref = { 1.0, 1.0, 1.0 };
	ufbx_prop *color = ufbx_find_prop(&light->props, "Color");
	ufbxt_assert(color && color->type == UFBX_PROP_COLOR);
	ufbxt_assert_close_vec3(err, color->value_vec3, color_ref);

	ufbx_prop *intensity = ufbx_find_prop(&light->props, "Intensity");
	ufbxt_assert(intensity && intensity->type == UFBX_PROP_NUMBER);
	ufbxt_assert_close_real(err, intensity->value_real, 100.0);

	// Model properties
	ufbx_vec3 translation_ref = { 4.076245307922363, 5.903861999511719, -1.0054539442062378 };
	ufbx_prop *translation = ufbx_find_prop(&node->props, "Lcl Translation");
	ufbxt_assert(translation && translation->type == UFBX_PROP_TRANSLATION);
	ufbxt_assert_close_vec3(err, translation->value_vec3, translation_ref);

	// Model defaults
	ufbx_vec3 scaling_ref = { 1.0, 1.0, 1.0 };
	ufbx_prop *scaling = ufbx_find_prop(&node->props, "GeometricScaling");
	ufbxt_assert(scaling && scaling->type == UFBX_PROP_VECTOR);
	ufbxt_assert_close_vec3(err, scaling->value_vec3, scaling_ref);
}
#endif

UFBXT_FILE_TEST(blender_282_suzanne)
#if UFBXT_IMPL
{
}
#endif

UFBXT_FILE_TEST(blender_282_suzanne_and_transform)
#if UFBXT_IMPL
{
}
#endif

UFBXT_FILE_TEST(maya_cube)
#if UFBXT_IMPL
{
	ufbxt_assert(scene->metadata.exporter == UFBX_EXPORTER_FBX_SDK);
	ufbxt_assert(scene->metadata.exporter_version == ufbx_pack_version(2019, 2, 0));

	ufbxt_assert(!strcmp(scene->metadata.original_application.vendor.data, "Autodesk"));
	ufbxt_assert(!strcmp(scene->metadata.original_application.name.data, "Maya"));
	ufbxt_assert(!strcmp(scene->metadata.original_application.version.data, "201900"));
	ufbxt_assert(!strcmp(scene->metadata.latest_application.vendor.data, "Autodesk"));
	ufbxt_assert(!strcmp(scene->metadata.latest_application.name.data, "Maya"));
	ufbxt_assert(!strcmp(scene->metadata.latest_application.version.data, "201900"));

	ufbx_node *node = ufbx_find_node(scene, "pCube1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	for (size_t face_i = 0; face_i < mesh->num_faces; face_i++) {
		ufbx_face face = mesh->faces.data[face_i];
		for (size_t i = face.index_begin; i < face.index_begin + face.num_indices; i++) {
			ufbx_vec3 n = ufbx_get_vertex_vec3(&mesh->vertex_normal, i);
			ufbx_vec3 b = ufbx_get_vertex_vec3(&mesh->vertex_bitangent, i);
			ufbx_vec3 t = ufbx_get_vertex_vec3(&mesh->vertex_tangent, i);
			ufbxt_assert_close_real(err, ufbxt_dot3(n, n), 1.0);
			ufbxt_assert_close_real(err, ufbxt_dot3(b, b), 1.0);
			ufbxt_assert_close_real(err, ufbxt_dot3(t, t), 1.0);
			ufbxt_assert_close_real(err, ufbxt_dot3(n, b), 0.0);
			ufbxt_assert_close_real(err, ufbxt_dot3(n, t), 0.0);
			ufbxt_assert_close_real(err, ufbxt_dot3(b, t), 0.0);

			for (size_t j = 0; j < face.num_indices; j++) {
				ufbx_vec3 p0 = ufbx_get_vertex_vec3(&mesh->vertex_position, face.index_begin + j);
				ufbx_vec3 p1 = ufbx_get_vertex_vec3(&mesh->vertex_position, face.index_begin + (j + 1) % face.num_indices);
				ufbx_vec3 edge;
				edge.x = p1.x - p0.x;
				edge.y = p1.y - p0.y;
				edge.z = p1.z - p0.z;
				ufbxt_assert_close_real(err, ufbxt_dot3(edge, edge), 1.0);
				ufbxt_assert_close_real(err, ufbxt_dot3(n, edge), 0.0);
			}

		}
	}
}
#endif

UFBXT_FILE_TEST(maya_color_sets)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "pCube1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->color_sets.count == 4);
	ufbxt_assert(!strcmp(mesh->color_sets.data[0].name.data, "RGBCube"));
	ufbxt_assert(!strcmp(mesh->color_sets.data[1].name.data, "White"));
	ufbxt_assert(!strcmp(mesh->color_sets.data[2].name.data, "Black"));
	ufbxt_assert(!strcmp(mesh->color_sets.data[3].name.data, "Alpha"));

	for (size_t i = 0; i < mesh->num_indices; i++) {
		ufbx_vec3 pos = ufbx_get_vertex_vec3(&mesh->vertex_position, i);
		ufbx_vec4 refs[4] = {
			{ 0.0, 0.0, 0.0, 1.0 },
			{ 1.0, 1.0, 1.0, 1.0 },
			{ 0.0, 0.0, 0.0, 1.0 },
			{ 1.0, 1.0, 1.0, 0.0 },
		};

		refs[0].x = pos.x + 0.5;
		refs[0].y = pos.y + 0.5;
		refs[0].z = pos.z + 0.5;
		refs[3].w = (pos.x + 0.5) * 0.1 + (pos.y + 0.5) * 0.2 + (pos.z + 0.5) * 0.4;

		for (size_t set_i = 0; set_i < 4; set_i++) {
			ufbx_vec4 color = ufbx_get_vertex_vec4(&mesh->color_sets.data[set_i].vertex_color, i);
			ufbxt_assert_close_vec4(err, color, refs[set_i]);
		}
	}
}
#endif

UFBXT_FILE_TEST(maya_uv_sets)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "pCube1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->uv_sets.count == 3);
	ufbxt_assert(!strcmp(mesh->uv_sets.data[0].name.data, "Default"));
	ufbxt_assert(!strcmp(mesh->uv_sets.data[1].name.data, "PerFace"));
	ufbxt_assert(!strcmp(mesh->uv_sets.data[2].name.data, "Row"));

	size_t counts1[2][2] = { 0 };
	size_t counts2[7][2] = { 0 };

	for (size_t i = 0; i < mesh->num_indices; i++) {
		ufbx_vec2 uv0 = ufbx_get_vertex_vec2(&mesh->uv_sets.data[0].vertex_uv, i);
		ufbx_vec2 uv1 = ufbx_get_vertex_vec2(&mesh->uv_sets.data[1].vertex_uv, i);
		ufbx_vec2 uv2 = ufbx_get_vertex_vec2(&mesh->uv_sets.data[2].vertex_uv, i);

		ufbxt_assert(uv0.x > 0.05f && uv0.y > 0.05f && uv0.x < 0.95f && uv0.y < 0.95f);
		int x1 = (int)(uv1.x + 0.5f), y1 = (int)(uv1.y + 0.5f);
		int x2 = (int)(uv2.x + 0.5f), y2 = (int)(uv2.y + 0.5f);
		ufbxt_assert_close_real(err, uv1.x - (ufbx_real)x1, 0.0);
		ufbxt_assert_close_real(err, uv1.y - (ufbx_real)y1, 0.0);
		ufbxt_assert_close_real(err, uv2.x - (ufbx_real)x2, 0.0);
		ufbxt_assert_close_real(err, uv2.y - (ufbx_real)y2, 0.0);
		ufbxt_assert(x1 >= 0 && x1 <= 1 && y1 >= 0 && y1 <= 1);
		ufbxt_assert(x2 >= 0 && x2 <= 6 && y2 >= 0 && y2 <= 1);
		counts1[x1][y1]++;
		counts2[x2][y2]++;
	}

	ufbxt_assert(counts1[0][0] == 6);
	ufbxt_assert(counts1[0][1] == 6);
	ufbxt_assert(counts1[1][0] == 6);
	ufbxt_assert(counts1[1][1] == 6);

	for (size_t i = 0; i < 7; i++) {
		size_t n = (i == 0 || i == 6) ? 1 : 2;
		ufbxt_assert(counts2[i][0] == n);
		ufbxt_assert(counts2[i][1] == n);
	}
}
#endif

UFBXT_FILE_TEST(blender_279_color_sets)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "Cube");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->color_sets.count == 3);
	ufbxt_assert(!strcmp(mesh->color_sets.data[0].name.data, "RGBCube"));
	ufbxt_assert(!strcmp(mesh->color_sets.data[1].name.data, "White"));
	ufbxt_assert(!strcmp(mesh->color_sets.data[2].name.data, "Black"));

	for (size_t i = 0; i < mesh->num_indices; i++) {
		ufbx_vec3 pos = ufbx_get_vertex_vec3(&mesh->vertex_position, i);
		ufbx_vec4 refs[3] = {
			{ 0.0, 0.0, 0.0, 1.0 },
			{ 1.0, 1.0, 1.0, 1.0 },
			{ 0.0, 0.0, 0.0, 1.0 },
		};

		refs[0].x = pos.x + 0.5;
		refs[0].y = pos.y + 0.5;
		refs[0].z = pos.z + 0.5;

		for (size_t set_i = 0; set_i < 3; set_i++) {
			ufbx_vec4 color = ufbx_get_vertex_vec4(&mesh->color_sets.data[set_i].vertex_color, i);
			ufbxt_assert_close_vec4(err, color, refs[set_i]);
		}
	}
}
#endif

UFBXT_FILE_TEST(blender_279_uv_sets)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "Cube");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->uv_sets.count == 3);
	ufbxt_assert(!strcmp(mesh->uv_sets.data[0].name.data, "Default"));
	ufbxt_assert(!strcmp(mesh->uv_sets.data[1].name.data, "PerFace"));
	ufbxt_assert(!strcmp(mesh->uv_sets.data[2].name.data, "Row"));

	size_t counts1[2][2] = { 0 };
	size_t counts2[7][2] = { 0 };

	for (size_t i = 0; i < mesh->num_indices; i++) {
		ufbx_vec2 uv0 = ufbx_get_vertex_vec2(&mesh->uv_sets.data[0].vertex_uv, i);
		ufbx_vec2 uv1 = ufbx_get_vertex_vec2(&mesh->uv_sets.data[1].vertex_uv, i);
		ufbx_vec2 uv2 = ufbx_get_vertex_vec2(&mesh->uv_sets.data[2].vertex_uv, i);

		ufbxt_assert(uv0.x > 0.05f && uv0.y > 0.05f && uv0.x < 0.95f && uv0.y < 0.95f);
		int x1 = (int)(uv1.x + 0.5f), y1 = (int)(uv1.y + 0.5f);
		int x2 = (int)(uv2.x + 0.5f), y2 = (int)(uv2.y + 0.5f);
		ufbxt_assert_close_real(err, uv1.x - (ufbx_real)x1, 0.0);
		ufbxt_assert_close_real(err, uv1.y - (ufbx_real)y1, 0.0);
		ufbxt_assert_close_real(err, uv2.x - (ufbx_real)x2, 0.0);
		ufbxt_assert_close_real(err, uv2.y - (ufbx_real)y2, 0.0);
		ufbxt_assert(x1 >= 0 && x1 <= 1 && y1 >= 0 && y1 <= 1);
		ufbxt_assert(x2 >= 0 && x2 <= 6 && y2 >= 0 && y2 <= 1);
		counts1[x1][y1]++;
		counts2[x2][y2]++;
	}

	ufbxt_assert(counts1[0][0] == 6);
	ufbxt_assert(counts1[0][1] == 6);
	ufbxt_assert(counts1[1][0] == 6);
	ufbxt_assert(counts1[1][1] == 6);

	for (size_t i = 0; i < 7; i++) {
		size_t n = (i == 0 || i == 6) ? 1 : 2;
		ufbxt_assert(counts2[i][0] == n);
		ufbxt_assert(counts2[i][1] == n);
	}
}
#endif

UFBXT_FILE_TEST(synthetic_sets_reorder)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "pCube1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->color_sets.count == 4);
	ufbxt_assert(!strcmp(mesh->color_sets.data[0].name.data, "RGBCube"));
	ufbxt_assert(!strcmp(mesh->color_sets.data[1].name.data, "White"));
	ufbxt_assert(!strcmp(mesh->color_sets.data[2].name.data, "Black"));
	ufbxt_assert(!strcmp(mesh->color_sets.data[3].name.data, "Alpha"));
	ufbxt_assert(!strcmp(mesh->uv_sets.data[0].name.data, "Default"));
	ufbxt_assert(!strcmp(mesh->uv_sets.data[1].name.data, "PerFace"));
	ufbxt_assert(!strcmp(mesh->uv_sets.data[2].name.data, "Row"));
}
#endif

UFBXT_FILE_TEST(maya_cone)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "pCone1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->vertex_crease.exists);
	ufbxt_assert(mesh->edges.count);
	ufbxt_assert(mesh->edge_crease.count);
	ufbxt_assert(mesh->edge_smoothing.count);

	ufbxt_assert(mesh->faces.data[0].num_indices == 16);

	for (size_t i = 0; i < mesh->num_indices; i++) {
		ufbx_vec3 pos = ufbx_get_vertex_vec3(&mesh->vertex_position, i);
		ufbx_real crease = ufbx_get_vertex_real(&mesh->vertex_crease, i);

		ufbxt_assert_close_real(err, crease, pos.y > 0.0 ? 0.998 : 0.0);
	}

	for (size_t i = 0; i < mesh->num_edges; i++) {
		ufbx_edge edge = mesh->edges.data[i];
		ufbx_real crease = mesh->edge_crease.data[i];
		bool smoothing = mesh->edge_smoothing.data[i];
		ufbx_vec3 a = ufbx_get_vertex_vec3(&mesh->vertex_position, edge.a);
		ufbx_vec3 b = ufbx_get_vertex_vec3(&mesh->vertex_position, edge.b);

		if (a.y < 0.0 && b.y < 0.0) {
			ufbxt_assert_close_real(err, crease, 0.583);
			ufbxt_assert(!smoothing);
		} else {
			ufbxt_assert(a.y > 0.0 || b.y > 0.0);
			ufbxt_assert_close_real(err, crease, 0.0);
			ufbxt_assert(smoothing);
		}
	}
}
#endif

#if UFBXT_IMPL
static void ufbxt_check_tangent_space(ufbxt_diff_error *err, ufbx_mesh *mesh)
{
	for (size_t set_i = 0; set_i < mesh->uv_sets.count; set_i++) {
		ufbx_uv_set set = mesh->uv_sets.data[set_i];
		ufbxt_assert(set.vertex_uv.exists);
		ufbxt_assert(set.vertex_bitangent.exists);
		ufbxt_assert(set.vertex_tangent.exists);

		for (size_t face_i = 0; face_i < mesh->num_faces; face_i++) {
			ufbx_face face = mesh->faces.data[face_i];

			for (size_t i = 0; i < face.num_indices; i++) {
				size_t a = face.index_begin + i;
				size_t b = face.index_begin + (i + 1) % face.num_indices;

				ufbx_vec3 pa = ufbx_get_vertex_vec3(&mesh->vertex_position, a);
				ufbx_vec3 pb = ufbx_get_vertex_vec3(&mesh->vertex_position, b);
				ufbx_vec3 ba = ufbx_get_vertex_vec3(&set.vertex_bitangent, a);
				ufbx_vec3 bb = ufbx_get_vertex_vec3(&set.vertex_bitangent, b);
				ufbx_vec3 ta = ufbx_get_vertex_vec3(&set.vertex_tangent, a);
				ufbx_vec3 tb = ufbx_get_vertex_vec3(&set.vertex_tangent, b);
				ufbx_vec2 ua = ufbx_get_vertex_vec2(&set.vertex_uv, a);
				ufbx_vec2 ub = ufbx_get_vertex_vec2(&set.vertex_uv, b);

				ufbx_vec3 dp = ufbxt_sub3(pb, pa);
				ufbx_vec2 du = ufbxt_sub2(ua, ub);

				ufbx_real dp_len = sqrt(ufbxt_dot3(dp, dp));
				dp.x /= dp_len;
				dp.y /= dp_len;
				dp.z /= dp_len;

				ufbx_real du_len = sqrt(ufbxt_dot2(du, du));
				du.x /= du_len;
				du.y /= du_len;

				ufbx_real dba = ufbxt_dot3(dp, ba);
				ufbx_real dbb = ufbxt_dot3(dp, bb);
				ufbx_real dta = ufbxt_dot3(dp, ta);
				ufbx_real dtb = ufbxt_dot3(dp, tb);
				ufbxt_assert_close_real(err, dba, dbb);
				ufbxt_assert_close_real(err, dta, dtb);

				ufbxt_assert_close_real(err, ub.x - ua.x, dta);
				ufbxt_assert_close_real(err, ub.y - ua.y, dba);
			}
		}
	}
}
#endif

UFBXT_FILE_TEST(maya_uv_set_tangents)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "pPlane1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_check_tangent_space(err, mesh);
}
#endif

UFBXT_FILE_TEST(blender_279_uv_set_tangents)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "Plane");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_check_tangent_space(err, mesh);
}
#endif

UFBXT_FILE_TEST(synthetic_tangents_reorder)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "pPlane1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_check_tangent_space(err, mesh);
}
#endif

UFBXT_FILE_TEST(blender_279_ball)
#if UFBXT_IMPL
{
	ufbx_material *red = (ufbx_material*)ufbx_find_element(scene, UFBX_ELEMENT_MATERIAL, "Red");
	ufbx_material *white = (ufbx_material*)ufbx_find_element(scene, UFBX_ELEMENT_MATERIAL, "White");
	ufbxt_assert(!strcmp(red->name.data, "Red"));
	ufbxt_assert(!strcmp(white->name.data, "White"));

	ufbx_vec3 red_ref = { 0.8, 0.0, 0.0 };
	ufbx_vec3 white_ref = { 0.8, 0.8, 0.8 };
	ufbxt_assert_close_vec3(err, red->fbx.diffuse_color.value_vec3, red_ref);
	ufbxt_assert_close_vec3(err, white->fbx.diffuse_color.value_vec3, white_ref);

	ufbx_node *node = ufbx_find_node(scene, "Icosphere");
	ufbxt_assert(node);
	ufbx_mesh *mesh = node->mesh;
	ufbxt_assert(mesh);
	ufbxt_assert(mesh->face_material.count);
	ufbxt_assert(mesh->face_smoothing.count);

	ufbxt_assert(mesh->materials.count == 2);
	ufbxt_assert(mesh->materials.data[0].material == red);
	ufbxt_assert(mesh->materials.data[1].material == white);

	for (size_t face_i = 0; face_i < mesh->num_faces; face_i++) {
		ufbx_face face = mesh->faces.data[face_i];
		ufbx_vec3 mid = { 0 };
		for (size_t i = 0; i < face.num_indices; i++) {
			mid = ufbxt_add3(mid, ufbx_get_vertex_vec3(&mesh->vertex_position, face.index_begin + i));
		}
		mid.x /= (ufbx_real)face.num_indices;
		mid.y /= (ufbx_real)face.num_indices;
		mid.z /= (ufbx_real)face.num_indices;

		bool smoothing = mesh->face_smoothing.data[face_i];
		int32_t material = mesh->face_material.data[face_i];
		ufbxt_assert(smoothing == (mid.x > 0.0));
		ufbxt_assert(material == (mid.z < 0.0 ? 1 : 0));
	}
}
#endif

UFBXT_FILE_TEST(synthetic_broken_material)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "pCube1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->materials.count == 0);
	ufbxt_assert(mesh->face_material.data == NULL);
}
#endif

UFBXT_FILE_TEST(maya_uv_and_color_sets)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "pCube1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->uv_sets.count == 2);
	ufbxt_assert(mesh->color_sets.count == 2);
	ufbxt_assert(!strcmp(mesh->uv_sets.data[0].name.data, "UVA"));
	ufbxt_assert(!strcmp(mesh->uv_sets.data[1].name.data, "UVB"));
	ufbxt_assert(!strcmp(mesh->color_sets.data[0].name.data, "ColorA"));
	ufbxt_assert(!strcmp(mesh->color_sets.data[1].name.data, "ColorB"));
}
#endif

UFBXT_FILE_TEST(maya_bad_face)
#if UFBXT_IMPL
{
	// TODO: Implement this if bad faces are trimmed
}
#endif

UFBXT_FILE_TEST(blender_279_edge_vertex)
#if UFBXT_IMPL
{
	// TODO: Implement this if bad faces are trimmed
}
#endif

UFBXT_FILE_TEST(blender_279_edge_circle)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "Circle");
	ufbxt_assert(node && node->mesh);
}
#endif

UFBXT_FILE_TEST(blender_293_instancing)
#if UFBXT_IMPL
{
	ufbxt_assert(scene->meshes.count == 1);
	ufbx_mesh *mesh = scene->meshes.data[0];
	ufbxt_assert(mesh->instances.count == 8);
}
#endif

UFBXT_FILE_TEST(synthetic_indexed_by_vertex)
#if UFBXT_IMPL
{
	ufbxt_assert(scene->meshes.count == 1);
	ufbx_mesh *mesh = scene->meshes.data[0];

	for (size_t vi = 0; vi < mesh->num_vertices; vi++) {
		int32_t ii = mesh->vertex_first_index.data[vi];
		ufbx_vec3 pos = mesh->vertex_position.values.data[mesh->vertex_position.indices.data[ii]];
		ufbx_vec3 normal = mesh->vertex_normal.values.data[mesh->vertex_normal.indices.data[ii]];
		ufbx_vec3 ref_normal = { 0.0f, pos.y > 0.0f ? 1.0f : -1.0f, 0.0f };
		ufbxt_assert_close_vec3(err, normal, ref_normal);
	}

	for (size_t ii = 0; ii < mesh->num_indices; ii++) {
		ufbx_vec3 pos = mesh->vertex_position.values.data[mesh->vertex_position.indices.data[ii]];
		ufbx_vec3 normal = mesh->vertex_normal.values.data[mesh->vertex_normal.indices.data[ii]];
		ufbx_vec3 ref_normal = { 0.0f, pos.y > 0.0f ? 1.0f : -1.0f, 0.0f };
		ufbxt_assert_close_vec3(err, normal, ref_normal);
	}

}
#endif

UFBXT_FILE_TEST(maya_lod_group)
#if UFBXT_IMPL
{
	{
		ufbx_node *node = ufbx_find_node(scene, "LOD_Group_1");
		ufbxt_assert(node);
		ufbxt_assert(node->attrib_type == UFBX_ELEMENT_LOD_GROUP);
		ufbx_lod_group *lod_group = (ufbx_lod_group*)node->attrib;
		ufbxt_assert(lod_group->element.type == UFBX_ELEMENT_LOD_GROUP);

		ufbxt_assert(node->children.count == 3);
		ufbxt_assert(lod_group->lod_levels.count == 3);

		ufbxt_assert(lod_group->relative_distances);
		ufbxt_assert(!lod_group->use_distance_limit);
		ufbxt_assert(!lod_group->ignore_parent_transform);

		ufbxt_assert(lod_group->lod_levels.data[0].display == UFBX_LOD_DISPLAY_USE_LOD);
		ufbxt_assert(lod_group->lod_levels.data[1].display == UFBX_LOD_DISPLAY_USE_LOD);
		ufbxt_assert(lod_group->lod_levels.data[2].display == UFBX_LOD_DISPLAY_USE_LOD);

		if (scene->metadata.version >= 7000) {
			ufbxt_assert_close_real(err, lod_group->lod_levels.data[0].distance, 100.0f);
			ufbxt_assert_close_real(err, lod_group->lod_levels.data[1].distance, 64.0f);
			ufbxt_assert_close_real(err, lod_group->lod_levels.data[2].distance, 32.0f);
		}
	}

	{
		ufbx_node *node = ufbx_find_node(scene, "LOD_Group_2");
		ufbxt_assert(node);
		ufbxt_assert(node->attrib_type == UFBX_ELEMENT_LOD_GROUP);
		ufbx_lod_group *lod_group = (ufbx_lod_group*)node->attrib;
		ufbxt_assert(lod_group->element.type == UFBX_ELEMENT_LOD_GROUP);

		ufbxt_assert(node->children.count == 3);
		ufbxt_assert(lod_group->lod_levels.count == 3);

		ufbxt_assert(!lod_group->relative_distances);
		ufbxt_assert(!lod_group->use_distance_limit);
		ufbxt_assert(lod_group->ignore_parent_transform);

		ufbxt_assert(lod_group->lod_levels.data[0].display == UFBX_LOD_DISPLAY_USE_LOD);
		ufbxt_assert(lod_group->lod_levels.data[1].display == UFBX_LOD_DISPLAY_SHOW);
		ufbxt_assert(lod_group->lod_levels.data[2].display == UFBX_LOD_DISPLAY_HIDE);

		if (scene->metadata.version >= 7000) {
			ufbxt_assert_close_real(err, lod_group->lod_levels.data[0].distance, 0.0f);
			ufbxt_assert_close_real(err, lod_group->lod_levels.data[1].distance, 4.520276f);
			ufbxt_assert_close_real(err, lod_group->lod_levels.data[2].distance, 18.081102f);
		}
	}
}
#endif

UFBXT_FILE_TEST(synthetic_missing_normals)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "pCube1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;
	ufbxt_assert(!mesh->vertex_normal.exists);
	ufbxt_assert(!mesh->skinned_normal.exists);
}
#endif

#if UFBXT_IMPL
static ufbx_load_opts ufbxt_generate_normals_opts()
{
	ufbx_load_opts opts = { 0 };
	opts.generate_missing_normals = true;
	return opts;
}
#endif

UFBXT_FILE_TEST_OPTS_ALT(synthetic_missing_normals_generated, synthetic_missing_normals, ufbxt_generate_normals_opts)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "pCube1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;
	ufbxt_assert(mesh->vertex_normal.exists);
	ufbxt_assert(mesh->skinned_normal.exists);

	for (size_t face_ix = 0; face_ix < mesh->faces.count; face_ix++) {
		ufbx_face face = mesh->faces.data[face_ix];
		ufbx_vec3 normal = ufbx_get_weighted_face_normal(&mesh->vertex_position, face);
		normal = ufbxt_normalize(normal);
		for (size_t i = 0; i < face.num_indices; i++) {
			ufbx_vec3 mesh_normal = ufbx_get_vertex_vec3(&mesh->vertex_normal, face.index_begin + i);
			ufbx_vec3 skinned_normal = ufbx_get_vertex_vec3(&mesh->skinned_normal, face.index_begin + i);
			ufbxt_assert_close_vec3(err, normal, mesh_normal);
			ufbxt_assert_close_vec3(err, normal, skinned_normal);
		}
	}
}
#endif

UFBXT_FILE_TEST(blender_279_nested_meshes)
#if UFBXT_IMPL
{
	// Diff to .obj file with nested objects and FBXASC escaped names
}
#endif

UFBXT_FILE_TEST(max_edge_visibility)
#if UFBXT_IMPL
{
	{
		ufbx_node *node = ufbx_find_node(scene, "Box001");
		ufbxt_assert(node && node->mesh);
		ufbx_mesh *mesh = node->mesh;
		ufbxt_assert(mesh->edge_visibility.count > 0);

		{
			size_t num_visible = 0;

			// Diagonal edges should be hidden
			for (size_t i = 0; i < mesh->num_edges; i++) {
				ufbx_edge edge = mesh->edges.data[i];
				ufbx_vec3 a = ufbx_get_vertex_vec3(&mesh->vertex_position, edge.a);
				ufbx_vec3 b = ufbx_get_vertex_vec3(&mesh->vertex_position, edge.b);
				ufbx_real len = ufbxt_length3(ufbxt_sub3(a, b));
				bool expected = len < 21.0f;
				bool visible = mesh->edge_visibility.data[i];
				ufbxt_assert(visible == expected);
				num_visible += visible ? 1u : 0u;
			}

			ufbxt_assert(mesh->num_edges == 18);
			ufbxt_assert(num_visible == 12);
		}

		ufbx_mesh *sub_mesh = ufbx_subdivide_mesh(mesh, 2, NULL, NULL);
		ufbxt_assert(sub_mesh);
		ufbxt_check_mesh(scene, sub_mesh);

		{
			size_t num_visible = 0;
			for (size_t i = 0; i < sub_mesh->num_edges; i++) {
				if (sub_mesh->edge_visibility.data[i]) {
					num_visible++;
				}
			}
			ufbxt_assert(num_visible == 12 * 4);
		}

		ufbx_free_mesh(sub_mesh);
	}

	{
		ufbx_node *node = ufbx_find_node(scene, "Cylinder001");
		ufbxt_assert(node && node->mesh);
		ufbx_mesh *mesh = node->mesh;
		ufbxt_assert(mesh->edge_visibility.count > 0);
		size_t num_visible = 0;

		// Diagonal and edges to the center should be hidden
		for (size_t i = 0; i < mesh->num_edges; i++) {
			ufbx_edge edge = mesh->edges.data[i];
			ufbx_vec3 a = ufbx_get_vertex_vec3(&mesh->vertex_position, edge.a);
			ufbx_vec3 b = ufbx_get_vertex_vec3(&mesh->vertex_position, edge.b);
			ufbx_vec2 a2 = { a.x, a.y };
			ufbx_vec2 b2 = { b.x, b.y };
			ufbx_real len = ufbxt_length3(ufbxt_sub3(a, b));
			ufbx_real len_a2 = ufbxt_length2(a2);
			ufbx_real len_b2 = ufbxt_length2(b2);
			bool expected = len < 20.7f && len_a2 > 0.1f && len_b2 > 0.1f;
			bool visible = mesh->edge_visibility.data[i];
			ufbxt_assert(visible == expected);
			num_visible += visible ? 1u : 0u;
		}

		ufbxt_assert(mesh->num_edges == 54);
		ufbxt_assert(num_visible == 27);
	}
}
#endif

UFBXT_FILE_TEST(zbrush_d20)
#if UFBXT_IMPL
{
	ufbxt_assert(scene->nodes.count == 3);

	{
		ufbx_node *node = ufbx_find_node(scene, "20 Sided");
		ufbxt_assert(node && node->mesh);
		ufbx_mesh *mesh = node->mesh;
		ufbxt_assert(mesh->num_faces == 20);
		ufbxt_assert(mesh->face_group.count == 20);
		for (int32_t i = 0; i < 20; i++) {
			ufbxt_assert(mesh->face_group.data[i] == 10 + i * 5);
		}
	}

	{
		ufbx_node *node = ufbx_find_node(scene, "PolyMesh3D1");
		ufbxt_assert(node && node->mesh);
		ufbx_mesh *mesh = node->mesh;
		ufbxt_assert(mesh->num_faces == 24);
		ufbxt_assert(mesh->face_group.count == 24);

		{
			uint32_t num_front = 0;
			for (size_t i = 0; i < 24; i++) {
				ufbx_face face = mesh->faces.data[i];
				ufbx_vec3 normal = ufbx_get_weighted_face_normal(&mesh->vertex_position, face);
				int32_t group = normal.z < 0.0f ? 9598 : 15349;
				num_front += normal.z < 0.0f ? 1 : 0;
				ufbxt_assert(mesh->face_group.data[i] == group);
			}
			ufbxt_assert(num_front == mesh->num_faces / 2);
		}

		ufbxt_assert(mesh->blend_deformers.count > 0);
		ufbx_blend_deformer *blend = mesh->blend_deformers.data[0];

		// WHAT? The 6100 version has duplicated blend shapes
		// and 7500 has duplicated blend deformers...
		if (scene->metadata.version == 6100) {
			ufbxt_assert(mesh->blend_deformers.count == 1);
			ufbxt_assert(blend->channels.count == 4);
		} else {
			ufbxt_assert(mesh->blend_deformers.count == 2);
			ufbxt_assert(blend->channels.count == 2);
		}

		// Check that poly groups work in subdivision
		ufbx_mesh *sub_mesh = ufbx_subdivide_mesh(mesh, 2, NULL, NULL);
		ufbxt_assert(sub_mesh);
		ufbxt_check_mesh(scene, sub_mesh);

		{
			uint32_t num_front = 0;
			ufbxt_assert(sub_mesh->face_group.count == sub_mesh->num_faces);
			for (size_t i = 0; i < sub_mesh->num_faces; i++) {
				ufbx_face face = sub_mesh->faces.data[i];
				ufbx_vec3 normal = ufbx_get_weighted_face_normal(&sub_mesh->vertex_position, face);
				int32_t group = normal.z < 0.0f ? 9598 : 15349;
				num_front += normal.z < 0.0f ? 1 : 0;
				ufbxt_assert(sub_mesh->face_group.data[i] == group);
			}
			ufbxt_assert(num_front == sub_mesh->num_faces / 2);
		}

		ufbx_free_mesh(sub_mesh);
	}
}
#endif

UFBXT_FILE_TEST(zbrush_d20_selection_set)
#if UFBXT_IMPL
{
	{
		ufbx_node *node = ufbx_find_node(scene, "PolyMesh3D1");
		ufbxt_assert(node && node->mesh);
		ufbx_mesh *mesh = node->mesh;

		for (size_t i = 0; i < 2; i++)
		{
			bool front = i == 0;
			const char *name = front ? "PolyMesh3D1_9598" : "PolyMesh3D1_15349";
			ufbx_selection_set *set = (ufbx_selection_set*)ufbx_find_element(scene, UFBX_ELEMENT_SELECTION_SET, name);
			ufbxt_assert(set);
			ufbxt_assert(set->nodes.count == 1);
			ufbx_selection_node *sel_node = set->nodes.data[0];

			ufbxt_assert(sel_node->target_node == node);
			ufbxt_assert(sel_node->target_mesh == mesh);
			ufbxt_assert(sel_node->faces.count == 12);

			for (size_t i = 0; i < sel_node->faces.count; i++) {
				uint32_t index = sel_node->faces.data[i];
				ufbxt_assert(index < mesh->faces.count);
				ufbx_face face = mesh->faces.data[index];
				ufbx_vec3 normal = ufbx_get_weighted_face_normal(&mesh->vertex_position, face);
				ufbxt_assert((normal.z < 0.0f) == front);
			}
		}
	}
}
#endif

UFBXT_FILE_TEST(maya_polygon_hole)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "pCube1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	ufbxt_assert(mesh->num_faces == 6);

	{
		size_t num_holes = 0;
		ufbxt_assert(mesh->face_hole.count == mesh->num_faces);
		for (size_t i = 0; i < mesh->num_faces; i++) {
			ufbx_face face = mesh->faces.data[i];
			ufbx_vec3 avg_pos = ufbx_zero_vec3;
			for (size_t j = 0; j < face.num_indices; j++) {
				ufbx_vec3 p = ufbx_get_vertex_vec3(&mesh->vertex_position, face.index_begin + j);
				avg_pos = ufbxt_add3(avg_pos, p);
			}
			avg_pos = ufbxt_mul3(avg_pos, 1.0f / (ufbx_real)face.num_indices);

			bool hole = fabs(avg_pos.y) > 0.49f;
			num_holes += hole ? 1 : 0;
			ufbxt_assert(mesh->face_hole.data[i] == hole);
		}
		ufbxt_assert(num_holes == 2);
	}

	ufbx_mesh *sub_mesh = ufbx_subdivide_mesh(mesh, 2, NULL, NULL);
	ufbxt_assert(sub_mesh);
	ufbxt_check_mesh(scene, sub_mesh);

	{
		size_t num_holes = 0;
		ufbxt_assert(sub_mesh->face_hole.count == sub_mesh->num_faces);
		for (size_t i = 0; i < sub_mesh->num_faces; i++) {
			ufbx_face face = sub_mesh->faces.data[i];
			ufbx_vec3 avg_pos = ufbx_zero_vec3;
			for (size_t j = 0; j < face.num_indices; j++) {
				ufbx_vec3 p = ufbx_get_vertex_vec3(&sub_mesh->vertex_position, face.index_begin + j);
				avg_pos = ufbxt_add3(avg_pos, p);
			}
			avg_pos = ufbxt_mul3(avg_pos, 1.0f / (ufbx_real)face.num_indices);

			bool hole = fabs(avg_pos.y) > 0.49f;
			num_holes += hole ? 1 : 0;
			ufbxt_assert(sub_mesh->face_hole.data[i] == hole);
		}
		ufbxt_assert(num_holes == 32);
	}

	ufbx_free_mesh(sub_mesh);
}
#endif

UFBXT_FILE_TEST_OPTS(synthetic_cursed_geometry, ufbxt_generate_normals_opts)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "pDisc1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;

	uint32_t indices[64];

	for (size_t i = 0; i < mesh->num_faces; i++) {
		ufbx_face face = mesh->faces.data[i];
		size_t num_tris = ufbx_triangulate_face(indices, ufbxt_arraycount(indices), mesh, face);
		ufbxt_assert(num_tris == 0 || num_tris == face.num_indices - 2);
	}

	ufbx_mesh *sub_mesh = ufbx_subdivide_mesh(mesh, 2, NULL, NULL);
	ufbx_free_mesh(sub_mesh);
}
#endif

UFBXT_FILE_TEST(synthetic_vertex_gaps)
#if UFBXT_IMPL
{
	ufbx_node *node = ufbx_find_node(scene, "pPlane1");
	ufbxt_assert(node && node->mesh);
	ufbx_mesh *mesh = node->mesh;
	ufbxt_assert(mesh->vertices.count == 7);

	ufbx_vec3 gap_values[] = {
		{ -1.0f, -1.1f, -1.2f },
		{ -2.0f, -2.1f, -2.2f },
		{ -3.0f, -3.1f, -3.2f },
		{ -4.0f, -4.1f, -4.2f },
	};

	ufbxt_assert_close_vec3(err, mesh->vertices.data[0], gap_values[0]);
	ufbxt_assert_close_vec3(err, mesh->vertices.data[3], gap_values[1]);
	ufbxt_assert_close_vec3(err, mesh->vertices.data[6], gap_values[2]);

	ufbxt_assert(mesh->vertex_normal.values.count == 8);
	ufbxt_assert_close_vec3(err, mesh->vertex_normal.values.data[0], gap_values[0]);
	ufbxt_assert_close_vec3(err, mesh->vertex_normal.values.data[3], gap_values[1]);
	ufbxt_assert_close_vec3(err, mesh->vertex_normal.values.data[6], gap_values[2]);
	ufbxt_assert_close_vec3(err, mesh->vertex_normal.values.data[7], gap_values[3]);

	ufbx_vec2 gap_uvs[] = {
		{ -1.3f, -1.4f },
		{ -2.3f, -2.4f },
		{ -3.3f, -3.4f },
		{ -4.3f, -4.4f },
	};

	ufbxt_assert(mesh->vertex_uv.values.count == 8);
	ufbxt_assert_close_vec2(err, mesh->vertex_uv.values.data[0], gap_uvs[0]);
	ufbxt_assert_close_vec2(err, mesh->vertex_uv.values.data[2], gap_uvs[1]);
	ufbxt_assert_close_vec2(err, mesh->vertex_uv.values.data[5], gap_uvs[2]);
	ufbxt_assert_close_vec2(err, mesh->vertex_uv.values.data[7], gap_uvs[3]);
}
#endif
