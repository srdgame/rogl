/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of The Open Ragnarok Project
    Copyright 2007 - 2012 The Open Ragnarok Team
    For the latest information visit http://www.open-ragnarok.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
*/
#include "gnd_gl.h"

#include "bmp.h"
#include "rogl_internal.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define GNDSURFACE_TOP 1
#define GNDSURFACE_FRONT 2
#define GNDSURFACE_RIGHT 3

void loadSurface(const struct ROGnd* gnd, struct ROGndGL *gndgl, unsigned int surface_id, unsigned int current_surface, unsigned int x, unsigned int y, unsigned int surface_side) {
	unsigned int idx, cell_idx;
	const struct ROGndCell *cell, *cell2;
	const struct ROGndSurface *surface;

	// For normal calculation
	float a[3], b[3], n[3];
	float normal_size;
	int i;

	// Some helping pointers
	idx = current_surface * 4;
	cell_idx = y * gnd->width + x;
	cell = &gnd->cells[cell_idx];
	surface = &gnd->surfaces[surface_id];

	// Texture
	gndgl->texturedata[current_surface] = surface->textureId;
	gndgl->vertexdata[idx+0].tex[0] = 1.0f - surface->u[0];
	gndgl->vertexdata[idx+0].tex[1] = surface->v[0];

	gndgl->vertexdata[idx+1].tex[0] = 1.0f - surface->u[1];
	gndgl->vertexdata[idx+1].tex[1] = surface->v[1];

	gndgl->vertexdata[idx+2].tex[0] = 1.0f - surface->u[2];
	gndgl->vertexdata[idx+2].tex[1] = surface->v[2];

	gndgl->vertexdata[idx+3].tex[0] = 1.0f - surface->u[3];
	gndgl->vertexdata[idx+3].tex[1] = surface->v[3];

	// Indices
	gndgl->indexdata[current_surface][0] = idx+0;
	gndgl->indexdata[current_surface][1] = idx+1;
	gndgl->indexdata[current_surface][2] = idx+3;
	gndgl->indexdata[current_surface][3] = idx+2;
    
	// Vertexes
	switch (surface_side) {
	case GNDSURFACE_TOP:
		gndgl->vertexdata[idx + 0].coord[0] = (float)x * gnd->zoom;
		gndgl->vertexdata[idx + 0].coord[1] = cell->height[0];
		gndgl->vertexdata[idx + 0].coord[2] = (float)y * gnd->zoom;

		gndgl->vertexdata[idx + 1].coord[0] = (float)(x + 1) * gnd->zoom;
		gndgl->vertexdata[idx + 1].coord[1] = cell->height[1];
		gndgl->vertexdata[idx + 1].coord[2] = (float)y * gnd->zoom;

		gndgl->vertexdata[idx + 2].coord[0] = (float)x * gnd->zoom;
		gndgl->vertexdata[idx + 2].coord[1] = cell->height[2];
		gndgl->vertexdata[idx + 2].coord[2] = (float)(y + 1) * gnd->zoom;

		gndgl->vertexdata[idx + 3].coord[0] = (float)(x + 1) * gnd->zoom;
		gndgl->vertexdata[idx + 3].coord[1] = cell->height[3];
		gndgl->vertexdata[idx + 3].coord[2] = (float)(y + 1) * gnd->zoom;
		break;

	case GNDSURFACE_FRONT:
		cell2 = &gnd->cells[(y+1) * gnd->width + x];
		gndgl->vertexdata[idx + 0].coord[0] = (float)x * gnd->zoom;
		gndgl->vertexdata[idx + 0].coord[1] = cell->height[2];
		gndgl->vertexdata[idx + 0].coord[2] = (float)(y + 1) * gnd->zoom;

		gndgl->vertexdata[idx + 1].coord[0] = (float)(x + 1) * gnd->zoom;
		gndgl->vertexdata[idx + 1].coord[1] = cell->height[3];
		gndgl->vertexdata[idx + 1].coord[2] = (float)(y + 1) * gnd->zoom;

		gndgl->vertexdata[idx + 2].coord[0] = (float)x * gnd->zoom;
		gndgl->vertexdata[idx + 2].coord[1] = cell2->height[0];
		gndgl->vertexdata[idx + 2].coord[2] = (float)(y + 1) * gnd->zoom;

		gndgl->vertexdata[idx + 3].coord[0] = (float)(x + 1) * gnd->zoom;
		gndgl->vertexdata[idx + 3].coord[1] = cell2->height[1];
		gndgl->vertexdata[idx + 3].coord[2] = (float)(y + 1) * gnd->zoom;
		break;

	case GNDSURFACE_RIGHT:
		cell2 = &gnd->cells[y * gnd->width + x + 1];
		
		gndgl->vertexdata[idx + 0].coord[0] = (float)(x + 1) * gnd->zoom;
		gndgl->vertexdata[idx + 0].coord[1] = cell->height[3];
		gndgl->vertexdata[idx + 0].coord[2] = (float)(y + 1) * gnd->zoom;

		gndgl->vertexdata[idx + 1].coord[0] = (float)(x + 1) * gnd->zoom;
		gndgl->vertexdata[idx + 1].coord[1] = cell->height[1];
		gndgl->vertexdata[idx + 1].coord[2] = (float)y * gnd->zoom;

		gndgl->vertexdata[idx + 2].coord[0] = (float)(x + 1) * gnd->zoom;
		gndgl->vertexdata[idx + 2].coord[1] = cell2->height[2];
		gndgl->vertexdata[idx + 2].coord[2] = (float)(y + 1) * gnd->zoom;

		gndgl->vertexdata[idx + 3].coord[0] = (float)(x + 1) * gnd->zoom;
		gndgl->vertexdata[idx + 3].coord[1] = cell2->height[0];
		gndgl->vertexdata[idx + 3].coord[2] = (float)y * gnd->zoom;
		break;
	}

	// Normals
	for (i = 0; i < 3; i++) {
		a[i] = gndgl->vertexdata[idx + 1].coord[i] - gndgl->vertexdata[idx + 0].coord[i];
		b[i] = gndgl->vertexdata[idx + 2].coord[i] - gndgl->vertexdata[idx + 0].coord[i];
	}

	n[0] = a[1] * b[2] - a[2] * b[1];
	n[1] = a[2] * b[0] - a[0] * b[2];
	n[2] = a[0] * b[1] - a[1] * b[0];

	normal_size = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);

	for (i = 0; i < 4; i++) {
		gndgl->vertexdata[idx + i].normal[0] = n[0] / normal_size;
		gndgl->vertexdata[idx + i].normal[1] = n[1] / normal_size;
		gndgl->vertexdata[idx + i].normal[2] = n[2] / normal_size;
	}
}

void swapQuads(struct ROGndGL *gndgl, unsigned int a, unsigned int b) {
	t_rogndidx aux;
	unsigned int id;

	// Swap indices
	aux[0] = gndgl->indexdata[a][0];
	aux[1] = gndgl->indexdata[a][1];
	aux[2] = gndgl->indexdata[a][2];
	aux[3] = gndgl->indexdata[a][3];

	gndgl->indexdata[a][0] = gndgl->indexdata[b][0];
	gndgl->indexdata[a][1] = gndgl->indexdata[b][1];
	gndgl->indexdata[a][2] = gndgl->indexdata[b][2];
	gndgl->indexdata[a][3] = gndgl->indexdata[b][3];

	gndgl->indexdata[b][0] = aux[0];
	gndgl->indexdata[b][1] = aux[1];
	gndgl->indexdata[b][2] = aux[2];
	gndgl->indexdata[b][3] = aux[3];

	// Swap texture id
	id = gndgl->texturedata[a];
	gndgl->texturedata[a] = gndgl->texturedata[b];
	gndgl->texturedata[b] = id;
}

void sortTextures(struct ROGndGL *gndgl) {
	unsigned int i, j;

	for (i = 0; i < gndgl->objcount; i++) {
		for (j = 0; j < gndgl->objcount - 1; j++) {
			if (gndgl->texturedata[j] > gndgl->texturedata[j+1]) {
				swapQuads(gndgl, j, j+1);
			}
		}
	}
}

struct ROGndGL *gndGL_load(const struct ROGnd* gnd) {
	struct ROGndGL *gndgl;
	unsigned int x, y, idx;
	unsigned int cur_surface;

	gndgl = (struct ROGndGL*)malloc(sizeof(struct ROGndGL));
	memset(gndgl, 0, sizeof(struct ROGndGL));
	gndgl->objcount = 0;

	// Iterate each cell to see how many surfaces it have
	for (x = 0; x < gnd->width; x++) {
		for (y = 0; y < gnd->height; y++) {
			idx = y * gnd->width + x;
			if (gnd->cells[idx].topSurfaceId != -1) gndgl->objcount++;
			if (gnd->cells[idx].frontSurfaceId != -1) gndgl->objcount++;
			if (gnd->cells[idx].rightSurfaceId != -1) gndgl->objcount++;
		}
	}
	printf("Let's draw %d quads!\n", gndgl->objcount);

	// Initialize memory
	gndgl->vertexdata = (struct RoGndGL_VertexInfo*)malloc(sizeof(struct RoGndGL_VertexInfo) * gndgl->objcount * 4);
	gndgl->indexdata = (t_rogndidx*)malloc(sizeof(t_rogndidx) * gndgl->objcount);
	gndgl->texturedata = (unsigned int*)malloc(sizeof(unsigned int) * gndgl->objcount);

	// Load vertex information into the structure
	cur_surface = 0;
	for (y = 0; y < gnd->height; y++) {
		for (x = 0; x < gnd->width; x++) {
			idx = y * gnd->width + x;
			if (gnd->cells[idx].topSurfaceId != -1) {
				loadSurface(gnd, gndgl, gnd->cells[idx].topSurfaceId, cur_surface, x, y, GNDSURFACE_TOP);
				cur_surface++;
			}
			if (gnd->cells[idx].frontSurfaceId != -1) {
				loadSurface(gnd, gndgl, gnd->cells[idx].frontSurfaceId, cur_surface, x, y, GNDSURFACE_FRONT);
				cur_surface++;
			}
			if (gnd->cells[idx].rightSurfaceId != -1) {
				loadSurface(gnd, gndgl, gnd->cells[idx].rightSurfaceId, cur_surface, x, y, GNDSURFACE_RIGHT);
				cur_surface++;
			}
		}
	}

	// Sort, based on textures
	sortTextures(gndgl);

	return(gndgl);
}

void gndGL_free(struct ROGndGL* gndgl) {
	free(gndgl->indexdata);
	free(gndgl->texturedata);
	free(gndgl->vertexdata);
	free(gndgl);
}

int loadTexture(const struct ROGrf* grf, const char* tex_fn, unsigned int glidx) {
	unsigned char m_alloc;
	struct strBMP *bmp;
	unsigned char *buf;
	struct ROGrfFile *file;
	char fn[64];
	unsigned int i, fn_size;

	sprintf(fn, "data\\texture\\%s", tex_fn);
	// Convert to Lowercase
	fn_size = strlen(fn);
	for (i = 0; i < fn_size; i++)
		fn[i] = (fn[i] >= 65 && fn[i] <= 90)?(fn[i]+32):fn[i];

	file = grf_getfileinfobyname(grf, fn);

	if (file == NULL)
		return(-1);

	if (file->data == NULL) {
		m_alloc = 1;
		grf_getdata(file);
	}
	else {
		m_alloc = 0;
	}
	
	bmp = loadBitmap(file->data, file->uncompressedLength);
	switch(bmp->info.bpp) {
	case 8:
		buf = bmp8to32(bmp);
		break;
	case 32:
		buf = (unsigned char*)bmp->data;
		break;
	default:
		// invalid BPP
		freeBitmap(bmp);
		if (m_alloc == 1)
			grf_freedata(file);
		return(-2);
	}

	glBindTexture(GL_TEXTURE_2D, glidx);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // Linear Min Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // Linear Mag Filter
	glTexImage2D(GL_TEXTURE_2D, 0, 4, bmp->info.width, bmp->info.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, buf);	

	if (bmp->info.bpp != 32) {
		freeBitmapData(buf);
	}
	freeBitmap(bmp);
	if (m_alloc == 1)
		grf_freedata(file);

	return(0);
}

struct ROGndGLVBO *gndGLVBO_load(const struct ROGndGL* gndgl, const struct ROGnd* m_gnd, const struct ROGrf* grf) {
	struct ROGndGLVBO *gnd;
	unsigned int i, idx, r;

	gnd = (struct ROGndGLVBO*)malloc(sizeof(struct ROGndGLVBO));
	memset(gnd, 0, sizeof(struct ROGndGLVBO));
	gnd->objcount = gndgl->objcount;

	// Register textures
	gnd->texturecount = m_gnd->texturecount;
	gnd->texturesids = (unsigned int*)malloc(sizeof(unsigned int) * gnd->texturecount);
	glGenTextures(gnd->texturecount, gnd->texturesids);
	for (i = 0; i < gnd->texturecount; i++) {
		r = loadTexture(grf, m_gnd->textures[i], gnd->texturesids[i]);
#ifdef DEBUG
		if (r == -1) {
			printf("ERROR: Texture %s not found.\n", m_gnd->textures[i]);
		}
		else if (r == -2) {
			printf("ERROR: Texture %s not supported\n.", m_gnd->textures[i]);
		}
#endif
	}

	// Copy stuff into video board buffers
	glGenBuffers(2, gnd->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gnd->vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(t_rogndidx) * gndgl->objcount, gndgl->indexdata, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, gnd->vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct RoGndGL_VertexInfo) * gndgl->objcount * 4, gndgl->vertexdata, GL_STATIC_DRAW);


	// Setup counts
	gnd->vertexcount = (unsigned int*)malloc(sizeof(unsigned int) * gnd->texturecount);
	memset(gnd->vertexcount, 0, sizeof(unsigned int) * gnd->texturecount);
	gnd->vertexcount[0] = 1;
	idx = 0;
	for (i = 1; i < gndgl->objcount; i++) {
		if (gndgl->texturedata[i] != gndgl->texturedata[i-1])
			idx++;
		gnd->vertexcount[idx]++;
	}

	return(gnd);
}

void gndGLVBO_draw(const struct ROGndGLVBO* gnd) {
	unsigned int i;
	unsigned int start;

	glBindBuffer(GL_ARRAY_BUFFER, gnd->vbo[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gnd->vbo[1]);

	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(struct RoGndGL_VertexInfo), 0);
    glNormalPointer(GL_FLOAT, sizeof(struct RoGndGL_VertexInfo), (void*)(sizeof(float) * 5));

	// Textures
	glClientActiveTexture(GL_TEXTURE0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);    //Notice that after we call glClientActiveTexture, we enable the array
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct RoGndGL_VertexInfo), (void*)(sizeof(float) * 3));

	start = 0;
	for (i = 0; i < gnd->texturecount; i++) {
		glBindTexture(GL_TEXTURE_2D, gnd->texturesids[i]);
		glDrawRangeElements(GL_QUADS, 0, gnd->objcount * 4, gnd->vertexcount[i] * 4, GL_UNSIGNED_SHORT, (void*)(sizeof(unsigned short) * start));
		start += gnd->vertexcount[i] * 4;
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void gndGLVBO_free(struct ROGndGLVBO* vbo) {
	if (vbo == NULL)
		return;

	glDeleteBuffers(2, vbo->vbo);
	glDeleteTextures(vbo->texturecount, vbo->texturesids);

	free(vbo->texturesids);
	free(vbo->vertexcount);

	free(vbo);
}
