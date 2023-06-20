pdf14_push_transparency_group(pdf14_ctx	*ctx, gs_int_rect *rect,
                              bool isolated, bool knockout,
                              byte alpha, byte shape,
                              gs_blend_mode_t blend_mode, bool idle,
                              uint mask_id, int numcomps)
{
    pdf14_buf *tos = ctx->stack;
    pdf14_buf *buf, *backdrop;
    bool has_shape, has_tags;

    if_debug1m('v', ctx->memory,
               "[v]pdf14_push_transparency_group, idle = %d\n", idle);
    /* todo: fix this hack, which makes all knockout groups isolated.
       For the vast majority of files, there won't be any visible
       effects, but it still isn't correct. The pixel compositing code
       for non-isolated knockout groups gets pretty hairy, which is
       why this is here. */
    if (knockout)
        isolated = true;
    has_shape = tos->has_shape || tos->knockout;
    has_tags = tos->has_tags;
    /* We need to create this based upon the size of
    the color space + an alpha channel. NOT the device size
    or the previous ctx size. */
    /* The second parameter in pdf14_buf_new decides if we should
       add a GROUP alpha channel to the buffer.  If it is NOT isolated, then this
       buffer will be added.  If it is isolated, then the buffer will not be added.
       I question the redundancy here of the alpha and the group alpha channel,
       but that will need to be looked at later. */
    buf = pdf14_buf_new(rect, has_tags, !isolated, has_shape, idle,
                        numcomps+1, ctx->memory);
    if_debug4m('v', ctx->memory,
               "[v]base buf: %d x %d, %d color channels, %d planes\n",
               buf->rect.q.x, buf->rect.q.y, buf->n_chan, buf->n_planes);
    if (buf == NULL)
        return_error(gs_error_VMerror);
    buf->isolated = isolated;
    buf->knockout = knockout;
    buf->alpha = alpha;
    buf->shape = shape;
    buf->blend_mode = blend_mode;
    buf->mask_id = mask_id;
    buf->mask_stack = ctx->mask_stack; /* Save because the group rendering may
                                          set up another (nested) mask. */
    ctx->mask_stack = NULL; /* Clean the mask field for rendering this group.
                            See pdf14_pop_transparency_group how to handle it. */
    buf->saved = tos;
    ctx->stack = buf;
    if (buf->data == NULL)
        return 0;
    if (idle)
        return 0;
    backdrop = pdf14_find_backdrop_buf(ctx);
    if (backdrop == NULL)
        memset(buf->data, 0, buf->planestride * (buf->n_chan +
                                                 (buf->has_shape ? 1 : 0)));
    else
        pdf14_preserve_backdrop(buf, tos, has_shape);
#if RAW_DUMP

    /* Dump the current buffer to see what we have. */

    dump_raw_buffer(ctx->stack->rect.q.y-ctx->stack->rect.p.y,
                ctx->stack->rowstride, ctx->stack->n_planes,
                ctx->stack->planestride, ctx->stack->rowstride,
                "TransGroupPush",ctx->stack->data);

    global_index++;
#endif
    return 0;
}