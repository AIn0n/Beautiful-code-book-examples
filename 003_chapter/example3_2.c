
void quicksort(int l, int u)
{
    if (l >= u) return;
    swap(l, randint(l, u));
    m = l;
    for (i = l+1; i <= u; i++) {
        comps++;
        if (x[i] < x[l])
            swap(++m, i);
    }
    swap(l, m);
    quicksort(l, m-1);
    quicksort(m+1, u);
}