#include "SupercellSWF.h"

#include "flash/flash_tags.h"
#include <cmath>

namespace fs = std::filesystem;

namespace sc
{
	namespace flash {
#pragma region Loading
		void SupercellSWF::load(const fs::path& filepath)
		{
			current_file = filepath;
			use_external_texture = load_internal(filepath, false);

			if (use_external_texture)
			{
				fs::path basename = filepath.stem();
				fs::path dirname = filepath.parent_path();

				fs::path multi_resolution_path = dirname / fs::path(basename).concat(multi_resolution_suffix.string()).concat("_tex.sc");
				fs::path low_resolution_path = dirname / fs::path(basename).concat(low_resolution_suffix.string()).concat("_tex.sc");
				fs::path common_file_path = dirname / fs::path(basename).concat("_tex.sc");

				if (low_memory_usage_mode && use_low_resolution && fs::exists(low_resolution_path))
				{
					load_internal(low_resolution_path, true);
				}
				else if (use_multi_resolution && fs::exists(multi_resolution_path))
				{
					load_internal(multi_resolution_path, true);
				}
				else if (fs::exists(common_file_path))
				{
					load_internal(common_file_path, true);
				}
				else
				{
					throw Exception("Failed to load external texture file");
				}
			}

			stream.clear();
		}

		bool SupercellSWF::load_internal(const fs::path& filepath, bool is_texture)
		{
			stream.open_file(filepath);

			// Reading .sc file
			if (!is_texture)
			{
				uint16_t shapes_count = stream.read_unsigned_short();
				shapes.resize(shapes_count);

				uint16_t movie_clips_count = stream.read_unsigned_short();
				movieclips.resize(movie_clips_count);

				uint16_t textures_count = stream.read_unsigned_short();
				textures.resize(textures_count);

				uint16_t textfield_count = stream.read_unsigned_short();
				textfields.resize(textfield_count);

				uint16_t matrices_count = stream.read_unsigned_short();
				uint16_t colors_count = stream.read_unsigned_short();
				matrixBanks.resize(1, MatrixBank(matrices_count, colors_count));

				stream.seek(5, Stream::SeekMode::Add); // unused

				uint16_t exports_count = stream.read_unsigned_short();
				exports.resize(exports_count);

				for (ExportName& export_name : exports)
				{
					export_name.id = stream.read_unsigned_short();
				}

				for (ExportName& export_name : exports)
				{
					stream.read_string(export_name.name);
				}
			}
			if (load_tags()) {
				for (ExportName& export_name : exports) {
					getOriginalMovieClip(export_name.id, &export_name.name).name = &export_name.name;
				}
				return true;
			}
			else return false;
		}

		bool SupercellSWF::load_tags()
		{
			bool has_external_texture = false;

			uint16_t shapes_loaded = 0;
			uint16_t movieclips_loaded = 0;
			uint16_t textures_loaded = 0;
			uint16_t textfields_loaded = 0;
			uint8_t banks_loaded = 0;
			uint16_t matrices_loaded = 0;
			uint16_t colors_loaded = 0;
			uint16_t modifiers_loaded = 0;

			while (true)
			{
				uint8_t tag = stream.read_unsigned_byte();
				int32_t tag_length = stream.read_int();

				if (tag == TAG_END)
					break;

				if (tag_length < 0)
					throw Exception("Negative tag length");

				switch (tag)
				{
				case TAG_USE_MULTI_RES_TEXTURE:
					use_multi_resolution = true;
					break;

				case TAG_DISABLE_LOW_RES_TEXTURE:
					use_low_resolution = false;
					break;

				case TAG_USE_EXTERNAL_TEXTURE:
					has_external_texture = true;
					break;

				case TAG_TEXTURE_FILE_SUFFIXES:
					stream.read_string(multi_resolution_suffix);
					stream.read_string(low_resolution_suffix);
					break;

				case TAG_TEXTURE:
				case TAG_TEXTURE_2:
				case TAG_TEXTURE_3:
				case TAG_TEXTURE_4:
				case TAG_TEXTURE_5:
				case TAG_TEXTURE_6:
				case TAG_TEXTURE_7:
				case TAG_TEXTURE_8:
				case TAG_TEXTURE_9:
				case TAG_TEXTURE_10:
					if (textures.size() < textures_loaded) {
						throw Exception("Trying to load too many textures");
					}

					textures[textures_loaded].load(*this, tag, !has_external_texture);
					textures_loaded++;
					break;

				case TAG_MOVIE_CLIP_MODIFIERS_COUNT: {
					uint16_t modifiers_count = stream.read_unsigned_short();
					movieclip_modifiers.resize(modifiers_count);
					break;
				}

				case TAG_MOVIE_CLIP_MODIFIER:
				case TAG_MOVIE_CLIP_MODIFIER_2:
				case TAG_MOVIE_CLIP_MODIFIER_3:
					movieclip_modifiers[modifiers_loaded].load(*this, tag);
					modifiers_loaded++;
					break;

				case TAG_SHAPE:
				case TAG_SHAPE_2:
					if (shapes.size() < shapes_loaded) {
						throw Exception("Trying to load too many Shapes");
					}

					shapes[shapes_loaded].load(*this, tag);
					shapes_loaded++;
					break;

				case TAG_TEXT_FIELD:
				case TAG_TEXT_FIELD_2:
				case TAG_TEXT_FIELD_3:
				case TAG_TEXT_FIELD_4:
				case TAG_TEXT_FIELD_5:
				case TAG_TEXT_FIELD_6:
				case TAG_TEXT_FIELD_7:
				case TAG_TEXT_FIELD_8:
					if (textfields.size() < textfields_loaded) {
						throw Exception("Trying to load too many TextFields");
					}

					textfields[textfields_loaded].load(*this, tag);
					textfields_loaded++;
					break;

				case TAG_MATRIX_BANK:
					matrices_loaded = 0;
					colors_loaded = 0;
					banks_loaded++;
					{
						uint16_t matrix_count = stream.read_unsigned_short();
						uint16_t colors_count = stream.read_unsigned_short();
						matrixBanks.emplace_back(matrix_count, colors_count);
					}
					break;

				case TAG_MATRIX_2x3:
				case TAG_MATRIX_2x3_2:
					matrixBanks[banks_loaded].matrices[matrices_loaded].load(*this, tag);
					matrices_loaded++;
					break;

				case TAG_COLOR_TRANSFORM:
					matrixBanks[banks_loaded].color_transforms[colors_loaded].load(*this);
					colors_loaded++;
					break;

				case TAG_MOVIE_CLIP:
				case TAG_MOVIE_CLIP_2:
				case TAG_MOVIE_CLIP_3:
				case TAG_MOVIE_CLIP_4:
				case TAG_MOVIE_CLIP_5:
				case TAG_MOVIE_CLIP_6:
					if (movieclips.size() < movieclips_loaded) {
						throw Exception("Trying to load too many MovieClips");
					}

					movieclips[movieclips_loaded].load(*this, tag);
					movieclips_loaded++;
					break;

				default:
					stream.seek(tag_length, Stream::SeekMode::Add);
					break;
				}
			}

			return has_external_texture;
		}

#pragma endregion

#pragma region Saving
		void SupercellSWF::save(const fs::path& filepath, Signature signature)
		{
			current_file = filepath;
			if (matrixBanks.size() == 0) {
				matrixBanks.resize(1);
			}

			save_internal(false, false);
			stream.save_file(filepath, signature);

			if (use_external_texture) {
				fs::path basename = filepath.stem();
				fs::path dirname = filepath.parent_path();

				fs::path multi_resolution_path = dirname / fs::path(basename).concat(multi_resolution_suffix.string()).concat("_tex.sc");
				fs::path low_resolution_path = dirname / fs::path(basename).concat(low_resolution_suffix.string()).concat("_tex.sc");
				fs::path common_file_path = dirname / fs::path(basename).concat("_tex.sc");

				save_internal(true, false);
				stream.save_file(use_multi_resolution ? multi_resolution_path : common_file_path, signature);

				if (use_low_resolution)
				{
					save_internal(true, true);
					stream.save_file(low_resolution_path, signature);
				}
			}
		}

		void SupercellSWF::save_internal(bool is_texture, bool is_lowres)
		{
			if (!is_texture)
			{
				stream.write_unsigned_short(shapes.size());
				stream.write_unsigned_short(movieclips.size());
				stream.write_unsigned_short(textures.size());
				stream.write_unsigned_short(textfields.size());

				stream.write_unsigned_short(matrixBanks[0].matrices.size());
				stream.write_unsigned_short(matrixBanks[0].color_transforms.size());

				// unused 5 bytes
				stream.write_unsigned_byte(0);
				stream.write_int(0);

				stream.write_unsigned_short(exports.size());

				for (const ExportName& export_name : exports)
				{
					stream.write_unsigned_short(export_name.id);
				}

				for (const ExportName& export_name : exports)
				{
					stream.write_string(export_name.name);
				}

				save_tags();
			}
			else
			{
				save_textures(is_texture, is_lowres);
			}

			stream.write_tag_flag(TAG_END);
		}

		void SupercellSWF::save_tags()
		{
			if (use_external_texture) {
				if (
					multi_resolution_suffix.compare(MULTIRES_DEFAULT_SUFFIX) != 0 ||
					low_resolution_suffix.compare(LOWRES_DEFAULT_SUFFIX) != 0) {
					size_t position = stream.write_tag_header(TAG_TEXTURE_FILE_SUFFIXES);
					stream.write_string(multi_resolution_suffix);
					stream.write_string(low_resolution_suffix);
					stream.write_tag_final(position);
				}

				if (!use_low_resolution)
					stream.write_tag_flag(TAG_DISABLE_LOW_RES_TEXTURE);

				if (use_multi_resolution)
					stream.write_tag_flag(TAG_USE_MULTI_RES_TEXTURE);

				stream.write_tag_flag(TAG_USE_EXTERNAL_TEXTURE);
			}

			save_textures(!use_external_texture, false);

			if (movieclip_modifiers.size() > 0) {
				stream.write_unsigned_byte(TAG_MOVIE_CLIP_MODIFIERS_COUNT); // Tag
				stream.write_int(sizeof(uint16_t)); // Tag Size
				stream.write_unsigned_short(movieclip_modifiers.size());

				for (const MovieClipModifier& modifier : movieclip_modifiers)
				{
					size_t position = stream.write_tag_header(modifier.tag(*this));
					modifier.save(*this);
					stream.write_tag_final(position);
				}
			}

			for (const ShapeOriginal& shape : shapes)
			{
				size_t position = stream.write_tag_header(shape.tag(*this));
				shape.save(*this);
				stream.write_tag_final(position);
			}

			for (const TextFieldOriginal& textField : textfields)
			{
				size_t position = stream.write_tag_header(textField.tag(*this));
				textField.save(*this);
				stream.write_tag_final(position);
			}

			for (uint8_t i = 0; matrixBanks.size() > i; i++)
			{
				const MatrixBank& bank = matrixBanks[i];

				if (i != 0)
				{
					stream.write_unsigned_byte(bank.tag(*this)); // Tag
					stream.write_int(sizeof(uint16_t) * 2); // Tag Size
					stream.write_unsigned_short(bank.matrices.size());
					stream.write_unsigned_short(bank.color_transforms.size());
				}

				for (const Matrix2D& matrix : bank.matrices)
				{
					size_t position = stream.write_tag_header(matrix.tag(*this));
					matrix.save(*this);
					stream.write_tag_final(position);
				}

				for (const ColorTransform& color : bank.color_transforms)
				{
					size_t position = stream.write_tag_header(color.tag(*this));
					color.save(*this);
					stream.write_tag_final(position);
				}
			}

			for (const MovieClipOriginal& movieclip : movieclips)
			{
				size_t position = stream.write_tag_header(movieclip.tag(*this));
				movieclip.save(*this);
				stream.write_tag_final(position);
			}
		}

		void SupercellSWF::save_textures(bool has_data, bool is_lowres)
		{
			for (uint16_t i = 0; textures.size() > i; i++)
			{
				SWFTexture& texture = textures[i];

				size_t position = stream.write_tag_header(texture.tag(*this, has_data));
				if (use_external_texture_files && has_data)
				{
					texture.encoding(SWFTexture::TextureEncoding::KhronosTexture);

					// Path String In Tag
					fs::path output_filepath = current_file.parent_path();
					output_filepath /= current_file.stem();
					if (is_lowres)
					{
						output_filepath += low_resolution_suffix.string();
					}
					output_filepath += "_";
					output_filepath += std::to_string(i);
					output_filepath += ".zktx";

					{
						std::string texture_filename = output_filepath.filename().string();
						SWFString texture_path(texture_filename);
						stream.write_string(texture_path);
					}

					BufferStream input_data;
					texture.save_buffer(input_data, is_lowres);

					OutputFileStream output_file(output_filepath);

					ZstdCompressor::Props props;
					ZstdCompressor cctx(props);

					input_data.seek(0);
					cctx.compress(input_data, output_file);
				}
				texture.save(*this, has_data, is_lowres);
				stream.write_tag_final(position);
			}
		}
#pragma endregion

		uint16_t SupercellSWF::GetDisplayObjectID(SWFString& name)
		{
			for (ExportName& exportName : exports)
			{
				if (exportName.name == name)
				{
					return exportName.id;
				}
			}

			throw new Exception("Failed to get Display Object");
		}

		DisplayObjectOriginal& SupercellSWF::getOriginalDisplayObject(uint16_t id, SWFString* exportNameForDebug = nullptr)
		{
			for (ShapeOriginal& shape : shapes)
			{
				if (shape.id == id)
				{
					return shape;
				}
			}

			for (TextFieldOriginal& textfield : textfields)
			{
				if (textfield.id == id)
				{
					return textfield;
				}
			}

			for (MovieClipModifier& modifier : movieclip_modifiers)
			{
				if (modifier.id == id)
				{
					return modifier;
				}
			}

			for (MovieClipOriginal& movie : movieclips)
			{
				if (movie.id == id)
				{
					return movie;
				}
			}

			throw new Exception("Failed to get Display Object");
		}

		MovieClipOriginal& SupercellSWF::getOriginalMovieClip(uint16_t id, SWFString* exportNameForDebug = nullptr)
		{
			for (MovieClipOriginal& movie : movieclips)
			{
				if (movie.id == id)
				{
					return movie;
				}
			}

			throw new Exception("Unable to find MovieClip id %d from %s needed by export name %s", id, "ui.sc", (exportNameForDebug == nullptr ? "null" : exportNameForDebug->data()));
		}
	}
}