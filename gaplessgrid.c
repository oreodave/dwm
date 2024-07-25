void
gaplessgrid(Monitor *m) {
	unsigned int n, cols, rows, cn, rn, i, cx, cy, cw, ch;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++) ;
	if(n == 0)
		return;

	/* grid dimensions */
	for(cols = 0; cols <= n/2; cols++)
		if(cols*cols >= n)
			break;
	if(n == 5) /* set layout against the general calculation: not 1:2:2, but 2:3 */
		cols = 2;
	rows = n/cols;

	const int gap_size = m->pertag->gaps_current[m->pertag->curtag];
	/* window geometries */
	cw = cols ? m->ww / cols : m->ww;
	cn = 0; /* current column number */
	rn = 0; /* current row number */
	for(i = 0, c = nexttiled(m->clients); c; i++, c = nexttiled(c->next)) {
		if(i/rows + 1 > cols - n%cols)
			rows = n/cols + 1;
		ch = (rows ? m->wh / rows : m->wh) - gap_size;
		cx = (m->wx + cn*cw) + gap_size;
		cy = (m->wy + rn*ch) + gap_size;
		resize(c, cx + gap_size, cy + gap_size, cw - 2 * c->bw - gap_size, ch - 2 * c->bw - gap_size, False);
		rn++;
		if(rn >= rows) {
			rn = 0;
			cn++;
		}
	}
}
