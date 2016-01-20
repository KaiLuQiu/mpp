/*
 * Copyright 2010 Rockchip Electronics S.LSI Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define MODULE_TAG "mpi"

#include <string.h>

#include "rk_mpi.h"

#include "mpp_log.h"
#include "mpp_mem.h"
#include "mpi_impl.h"
#include "mpp.h"

static MPP_RET mpi_config(MppCtx ctx, MppEncConfig cfg)
{
    (void)ctx;
    (void)cfg;
    MPI_FUNCTION_ENTER();

    MPI_FUNCTION_LEAVE();
    return MPP_OK;
}

static MPP_RET mpi_decode(MppCtx ctx, MppPacket packet, MppFrame *frame)
{
    (void)ctx;
    (void)packet;
    (void)frame;
    MPI_FUNCTION_ENTER();

    MPI_FUNCTION_LEAVE();
    return MPP_OK;
}

static MPP_RET mpi_encode(MppCtx ctx, MppFrame frame, MppPacket *packet)
{
    (void)ctx;
    (void)packet;
    (void)frame;
    MPI_FUNCTION_ENTER();

    MPI_FUNCTION_LEAVE();
    return MPP_OK;
}

static MPP_RET mpi_decode_put_packet(MppCtx ctx, MppPacket packet)
{
    MPI_FUNCTION_ENTER();
    MpiImpl *p = (MpiImpl *)ctx;

    if (NULL == p || p->check != p || NULL == p->ctx || NULL == packet) {
        mpp_err_f("found invalid context input ctx %p packet %p\n",
                ctx, packet);
        return MPP_ERR_UNKNOW;
    }

    MPP_RET ret = p->ctx->put_packet(packet);

    MPI_FUNCTION_LEAVE();
    return ret;
}

static MPP_RET mpi_decode_get_frame(MppCtx ctx, MppFrame *frame)
{
    MPI_FUNCTION_ENTER();
    MpiImpl *p = (MpiImpl *)ctx;

    if (NULL == p || p->check != p || NULL == p->ctx || NULL == frame) {
        mpp_err_f("found invalid context input ctx %p frame %p\n",
                ctx, frame);
        return MPP_ERR_UNKNOW;
    }

    MPP_RET ret = p->ctx->get_frame(frame);

    MPI_FUNCTION_LEAVE();
    return ret;
}

static MPP_RET mpi_encode_put_frame(MppCtx ctx, MppFrame frame)
{
    MPI_FUNCTION_ENTER();
    MpiImpl *p = (MpiImpl *)ctx;

    if (NULL == p || p->check != p || NULL == p->ctx || NULL == frame) {
        mpp_err_f("found invalid context input ctx %p frame %p\n",
                ctx, frame);
        return MPP_ERR_UNKNOW;
    }

    MPP_RET ret = p->ctx->put_frame(frame);

    MPI_FUNCTION_LEAVE();
    return ret;
}

static MPP_RET mpi_encode_get_packet(MppCtx ctx, MppPacket *packet)
{
    MPI_FUNCTION_ENTER();
    MpiImpl *p = (MpiImpl *)ctx;

    if (NULL == p || p->check != p || NULL == p->ctx || NULL == packet) {
        mpp_err_f("found invalid context input ctx %p packet %p\n",
                ctx, packet);
        return MPP_ERR_UNKNOW;
    }

    MPP_RET ret = p->ctx->get_packet(packet);

    MPI_FUNCTION_LEAVE();
    return ret;
}

static MPP_RET mpi_flush(MppCtx ctx)
{
    MpiImpl *p = (MpiImpl *)ctx;
    MPP_RET ret = MPP_OK;
    MPI_FUNCTION_ENTER();
    if (p->ctx->mInitDone) {
        ret = p->ctx->reset();
    }
    MPI_FUNCTION_LEAVE();
    return ret;
}

static MPP_RET mpi_control(MppCtx ctx, MpiCmd cmd, MppParam param)
{
    MPI_FUNCTION_ENTER();
    MpiImpl *p = (MpiImpl *)ctx;
    MPP_RET ret = p->ctx->control(cmd, param);
    MPI_FUNCTION_LEAVE();
    return ret;
}

static MppApi mpp_api = {
    sizeof(mpp_api),
    1,
    mpi_config,
    mpi_decode,
    mpi_encode,
    mpi_decode_put_packet,
    mpi_decode_get_frame,
    mpi_encode_put_frame,
    mpi_encode_get_packet,
    mpi_flush,
    mpi_control,
    {0},
};

MPP_RET mpp_create(MppCtx *ctx, MppApi **mpi)
{
    MpiImpl *p;

    MPI_FUNCTION_ENTER();

    if (NULL == ctx || NULL == mpi) {
        mpp_err_f("invalid input ctx %p mpi %p\n", ctx, mpi);
        return MPP_ERR_NULL_PTR;
    }

    *ctx = NULL;
    *mpi = NULL;

    p = mpp_malloc(MpiImpl, 1);
    if (NULL == p) {
        mpp_err_f("failed to allocate context\n");
        return MPP_ERR_MALLOC;
    }

    memset(p, 0, sizeof(*p));
    p->ctx = new Mpp();
    if (NULL == p->ctx) {
        mpp_free(p);
        mpp_err_f("failed to new Mpp\n");
        return MPP_ERR_MALLOC;
    }
    p->api      = &mpp_api;
    p->check    = p;
    *ctx = p;
    *mpi = p->api;
    MPI_FUNCTION_LEAVE_OK();
    return MPP_OK;
}


MPP_RET mpp_init(MppCtx ctx, MppCtxType type, MppCodingType coding)
{
    MpiImpl *p = (MpiImpl*)ctx;
    MPI_FUNCTION_ENTER();

    if (NULL == ctx ||
        type >= MPP_CTX_BUTT ||
        coding >= MPP_VIDEO_CodingMax) {
        mpp_err_f("invalid input ctx %p type %d coding %d\n",
                ctx, type, coding);
        return MPP_ERR_NULL_PTR;
    }

    p->ctx->init(type, coding);
    p->type     = type;
    p->coding   = coding;

    get_mpi_debug();
    MPI_FUNCTION_LEAVE_OK();
    return MPP_OK;
}

MPP_RET mpp_destroy(MppCtx ctx)
{
    MpiImpl *p;
    MPI_FUNCTION_ENTER();

    if (NULL == ctx) {
        mpp_err_f("input ctx %p is null pointer\n", ctx);
        return MPP_ERR_NULL_PTR;
    }

    p = (MpiImpl*)ctx;

    if (p->check != p) {
        mpp_err_f("input invalid MppCtx\n");
        return MPP_ERR_UNKNOW;
    }

    if (p->ctx)
        delete p->ctx;

    mpp_free(p);

    MPI_FUNCTION_LEAVE();
    return MPP_OK;
}

