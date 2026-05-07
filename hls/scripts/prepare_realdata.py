from pathlib import Path
import math
import shutil

SRC_DIR = Path(r"D:/Thesis/workspaces/HLS01/data")
DST_DIR = Path(r"D:/Thesis/workspaces/HLS01/hls_realdata")
DST_DIR.mkdir(parents=True, exist_ok=True)

def read_scalar(name, typ=float):
    txt = (SRC_DIR / name).read_text().strip().split()
    return typ(txt[0])

def copy(src, dst):
    shutil.copyfile(SRC_DIR / src, DST_DIR / dst)

def qmult_shift(real_multiplier):
    x = float(real_multiplier)
    shift = 0
    while x < 0.5:
        x *= 2.0
        shift += 1
    mult = int(round(x * (1 << 31)))
    if mult == (1 << 31):
        mult //= 2
        shift -= 1
    return mult, shift

copy("q0_input_quant_out_u8.txt", "input.txt")
copy("l7b_res2_relu_out_out_u8.txt", "cnn_expected.txt")

copy("stem.0_weight_int8.txt", "stem_w.txt")
copy("stem.0_bias_int32.txt", "stem_b.txt")
copy("stem.0_multiplier.txt", "stem_m.txt")
copy("stem.0_shift.txt", "stem_s.txt")
copy("stem.0_weight_zp.txt", "stem_wzp.txt")

copy("res1.block.0_weight_int8.txt", "res1_w1.txt")
copy("res1.block.0_bias_int32.txt", "res1_b1.txt")
copy("res1.block.0_multiplier.txt", "res1_m1.txt")
copy("res1.block.0_shift.txt", "res1_s1.txt")
copy("res1.block.0_weight_zp.txt", "res1_wzp1.txt")

copy("res1.block.2_weight_int8.txt", "res1_w2.txt")
copy("res1.block.2_bias_int32.txt", "res1_b2.txt")
copy("res1.block.2_multiplier.txt", "res1_m2.txt")
copy("res1.block.2_shift.txt", "res1_s2.txt")
copy("res1.block.2_weight_zp.txt", "res1_wzp2.txt")

copy("down1.0_weight_int8.txt", "down_w.txt")
copy("down1.0_bias_int32.txt", "down_b.txt")
copy("down1.0_multiplier.txt", "down_m.txt")
copy("down1.0_shift.txt", "down_s.txt")
copy("down1.0_weight_zp.txt", "down_wzp.txt")

copy("res2.block.0_weight_int8.txt", "res2_w1.txt")
copy("res2.block.0_bias_int32.txt", "res2_b1.txt")
copy("res2.block.0_multiplier.txt", "res2_m1.txt")
copy("res2.block.0_shift.txt", "res2_s1.txt")
copy("res2.block.0_weight_zp.txt", "res2_wzp1.txt")

copy("res2.block.2_weight_int8.txt", "res2_w2.txt")
copy("res2.block.2_bias_int32.txt", "res2_b2.txt")
copy("res2.block.2_multiplier.txt", "res2_m2.txt")
copy("res2.block.2_shift.txt", "res2_s2.txt")
copy("res2.block.2_weight_zp.txt", "res2_wzp2.txt")

stem_zx = read_scalar("stem.0_Zx.txt", int)
stem_zy = read_scalar("stem.0_Zy.txt", int)

res1_c1_zx = read_scalar("res1.block.0_Zx.txt", int)
res1_c1_zy = read_scalar("res1.block.0_Zy.txt", int)
res1_c2_zx = read_scalar("res1.block.2_Zx.txt", int)
res1_c2_zy = read_scalar("res1.block.2_Zy.txt", int)

down_zx = read_scalar("down1.0_Zx.txt", int)
down_zy = read_scalar("down1.0_Zy.txt", int)

res2_c1_zx = read_scalar("res2.block.0_Zx.txt", int)
res2_c1_zy = read_scalar("res2.block.0_Zy.txt", int)
res2_c2_zx = read_scalar("res2.block.2_Zx.txt", int)
res2_c2_zy = read_scalar("res2.block.2_Zy.txt", int)

# residual add params
res1_main_s = read_scalar("res1.block.2_Sy.txt", float)
res1_skip_s = read_scalar("l0b_stem_relu_Sy.txt", float)
res1_add_s  = read_scalar("l3b_res1_relu_out_Sx.txt", float)

res1_main_z = read_scalar("res1.block.2_Zy.txt", int)
res1_skip_z = read_scalar("l0b_stem_relu_Zy.txt", int)
res1_add_z  = read_scalar("l3b_res1_relu_out_Zx.txt", int)

res1_ma, res1_sa = qmult_shift(res1_main_s / res1_add_s)
res1_mb, res1_sb = qmult_shift(res1_skip_s / res1_add_s)

res2_main_s = read_scalar("res2.block.2_Sy.txt", float)
res2_skip_s = read_scalar("l4b_down1_relu_Sy.txt", float)
res2_add_s  = read_scalar("l7b_res2_relu_out_Sx.txt", float)

res2_main_z = read_scalar("res2.block.2_Zy.txt", int)
res2_skip_z = read_scalar("l4b_down1_relu_Zy.txt", int)
res2_add_z  = read_scalar("l7b_res2_relu_out_Zx.txt", int)

res2_ma, res2_sa = qmult_shift(res2_main_s / res2_add_s)
res2_mb, res2_sb = qmult_shift(res2_skip_s / res2_add_s)

scalars = [
    stem_zx, stem_zy,
    res1_c1_zx, res1_c1_zy,
    res1_c2_zx, res1_c2_zy,
    down_zx, down_zy,
    res2_c1_zx, res2_c1_zy,
    res2_c2_zx, res2_c2_zy,

    res1_ma, res1_sa, res1_main_z,
    res1_mb, res1_sb, res1_skip_z,
    res1_add_z,

    res2_ma, res2_sa, res2_main_z,
    res2_mb, res2_sb, res2_skip_z,
    res2_add_z,
]

(DST_DIR / "scalars.txt").write_text("\n".join(str(x) for x in scalars) + "\n")

print("Prepared HLS real data in:", DST_DIR)