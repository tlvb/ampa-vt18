bool is_inside(const hitbox *h, screen_dim x, screen_dim y)
{
  return x >= h->pos[0] && x < h->pos[0]+h->size[0] &&
         y >= h->pos[1] && y < h->pos[1]+h->size[1];
}
