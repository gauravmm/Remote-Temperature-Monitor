import colormaps as cmaps

cmap = cmaps._inferno_data;

for i,r in enumerate(cmap):
    if i % 2 == 0:
        print "{%s}," % ",\t".join(str(int(round(v * 255))) for v in r);
