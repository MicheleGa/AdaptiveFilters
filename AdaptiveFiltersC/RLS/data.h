#define LENGTH 16
#define N_SAMPLES 256
#define SNR 15
#define TOL 100

#define RLS_LMBD 1.0f
#define RLS_DELTA 2.0f

#define W_RLS_INIT {-0.30869927, -1.68925781, 0.31823048, 0.20088213, 0.31728448, 0.09390153, 2.07554184, 0.47417999, 1.93396342, 1.2529477, -0.57476108, -0.47720872, -0.57509399, -0.28235504, -1.2097, 0.0}
#define W_RLS_CHECK {-0.07847367, -0.42942199, 0.08089657, 0.05106574, 0.08065609, 0.02387047, 0.52761829, 0.12054011, 0.49162799, 0.31850869, -0.14610857, -0.12131003, -0.1461932, -0.07177677, -0.3075, 0.0}
#define RLS_X {0.84541881, 0.00664922, -0.28840055, 0.81543026, -0.13616807, -0.98767868, -0.53579241, -0.7095552, -0.03218715, 1.9867591, 2.17712187, -0.8675339, -1.02893713, 0.7290332, -0.97115735, 0.78521505, -0.82890972, -0.32150648, 0.02993035, 1.77756592, -0.39598202, -0.10850478, 0.96424399, -0.62143999, 0.79028891, 0.10868437, -0.36709066, 2.54020367, -0.07870475, -0.15898442, -1.50269768, -0.57755795, 0.19329673, 0.17977447, 2.34903526, 0.75361331, -0.35810194, -1.86890545, -0.56463491, 0.99808756, -0.37179155, 0.76396272, -0.604468, -0.35074431, 0.38318984, -1.43191538, -0.34222165, -0.53472713, 0.16913202, 0.3512157, 2.3225533, 0.92763012, 0.38102712, -1.23164157, 1.67320677, 0.35820304, 1.80646902, 0.48884936, 0.50606033, -1.62499918, -1.04924981, 0.87626959, -0.3327983, -0.60554227, 0.23795701, -1.6364616, 0.41270096, -2.45726754, 0.2501871, -0.0303859, 0.53915618, -0.37358616, 0.63302397, -0.15545573, -0.29568596, 0.15930977, -1.07917077, 0.76265873, -0.28594833, -0.44646363, 0.89165747, -1.29469305, -0.87666274, 0.43866103, -0.07187422, -0.51386022, -0.06437953, -0.04814238, -1.43867516, 1.89306458, 0.3826015, 1.3534778, 0.20034292, -1.40553724, 0.03911527, -0.86097631, -0.76526418, -0.50745923, 0.71904222, 0.34517127, -0.74253973, -1.34756839, 1.49641224, 0.3036509, 0.48209439, 0.5058863, -0.38092206, 2.07572055, 0.17895369, 0.06375631, -1.56854227, -0.83667281, -0.2980136, 0.32728215, 0.32887072, 0.37298838, -0.40401466, 0.10831252, -0.47053331, 0.23522069, 0.21544179, 0.85835961, -0.42211118, -0.27464779, -0.38385712, -0.68778007, 0.30393683, 0.4464151, -0.99617307, 0.00230223, -0.05818095, -1.37869804, 1.33775311, 0.22421077, 1.15390391, 0.33371084, 0.21967017, -1.58422258, -0.32513658, -1.81242335, 0.60522223, -0.14670884, -0.48053105, 1.24171623, 1.69896697, 0.58805927, 0.7926541, -0.07542251, 0.42918802, 0.85777278, 0.8395671, 1.7449372, 0.020137, -1.3017934, -0.67525452, -2.17698358, -1.01327824, -0.40433458, -0.10078919, 1.19855559, 1.31899842, -1.01501209, -1.05184617, -0.52244887, -1.09106981, -1.92854202, 0.58574004, 1.33993535, 0.30195517, 1.22844848, -0.26587486, -1.75171848, -0.71738371, -0.18267962, 1.2484838, -1.13312759, 1.37647798, -1.02930882, -0.32187641, 1.3366511, 0.83114336, -0.38473572, 0.05366248, 0.31105579, 1.00664281, 0.85245695, -0.18410615, -2.02994102, -0.16286912, -0.36263331, 1.307525, 0.29452825, 0.6607745, 1.98104572, -1.0571879, 1.98768089, 0.07679354, -1.27991064, -1.0868517, -1.03811543, -0.19685427, 0.18777982, 1.06322836, 0.95433828, 0.64607387, 0.198949, 0.57722759, 0.64676626, -0.58058178, 1.17294044, 1.40645057, -2.09609551, 2.04038163, 0.36800259, -1.00143771, 1.00857534, 0.92966269, 0.33454709, -0.31060762, -0.23463586, 1.08113379, -0.59319403, 0.90351307, 0.63664476, 0.50844303, -2.30045409, -0.78449979, -0.29736699, -0.28132143, 0.1096852, -0.64790089, -0.55571739, 0.38598679, -0.72569623, -1.70702966, -1.20131594, -3.0127128, -0.81640179, 0.24738229, 1.08943758, -1.31236803, -0.44175977, -0.95782303, 1.6807171, 0.02515258, -0.02949351, -1.3006879, 1.26763721, 0.33638118, 0.04960524, 1.00141782, -0.12387613, -0.7043211, 0.53147797, 0.49486488, 0.2238}
#define RLS_INPUT {-0.16729863, -0.51208683, 0.34262095, 0.19774256, -0.28010908, 0.22169136, 0.98792614, 0.45802829, 0.34207307, 0.32266437, -1.14753813, -1.0541021, 0.2600524, -0.35221991, -0.92947767, 1.08930648, 0.93105783, 1.38437585, 1.46203436, 0.14590277, -2.25197472, 0.59697308, -0.9322567, -1.16262008, -0.25639881, 0.41170097, 0.18128489, 0.40418215, 0.15249588, -0.39515665, 1.07595439, 0.5602981, 0.07997684, 0.84508396, -0.60106839, -0.16598226, -0.5524908, 0.03741635, 0.27190129, -0.72641764, 0.88652352, 0.17422896, 1.07707676, 0.60631118, -0.40251479, -0.58482725, -0.98595436, 0.84141113, 0.1374541, -0.45173715, 0.15060968, -1.73613845, -0.17760465, -0.89972799, 0.1537846, -0.92505361, 1.31248419, 0.32484266, 1.72729583, 1.27850365, 1.92689969, 0.06190134, 0.71146771, 0.95128638, 0.17146202, -0.32083158, 0.09102296, -0.4692625, -1.14812511, -0.75996159, -0.23343031, -1.80946666, 0.22443267, -1.74793561, 0.23717777, -0.88078791, 0.16008327, 0.99633447, 0.22061386, 1.01310628, -0.02401486, 0.40811543, -0.28424454, 0.81337351, -1.12628791, -0.37689639, 0.73449417, -0.82712751, -0.0615569, -0.08699641, -1.72572685, -0.58636529, -0.22793874, 0.22541395, -0.62084214, 1.22554908, 0.53960559, 1.54956525, 1.39436831, -0.20097333, 0.138769, -1.24947017, -0.50978316, -2.27063189, -0.78988468, -0.56347986, 0.26397454, 0.12237083, -0.3395768, -0.03892502, 1.22990377, 1.92211311, 0.24176977, 1.6571754, 0.19761051, 0.99291871, -0.887396, -1.07944371, -1.52586354, -1.08039977, -0.43395744, -0.01058366, -0.17422689, 0.72105907, 0.39307548, 0.2302009, 0.07061399, 0.21304281, -0.21636503, 0.49509475, -0.07568931, -0.65755671, 0.15452414, -0.98051417, -0.87203617, -0.8863625, -0.05906624, -0.7702868, 1.76015261, -0.10566881, 1.57844936, 0.37864358, 1.1388129, -0.13044813, -1.05417925, -2.14303482, -1.4498278, -1.18339041, -0.43737445, 1.24996161, 1.07300305, 1.26970963, 1.52591187, 1.77651662, 0.87066166, 0.68238271, 1.41559989, 1.22460981, -0.11649479, -0.37204492, -1.7775032, -2.81942642, -1.25351822, -1.19409414, -0.95068754, 0.74636641, 2.02909842, 0.26358358, 0.27572461, -0.15216426, -1.9108739, -1.41803657, 0.21091187, -0.38060893, -0.22794483, 1.92506437, 1.55265393, -0.67987556, 0.43636716, -0.98604664, -1.41706585, -1.49488887, 1.42808171, -0.58033684, -0.0247574, 0.93298873, -0.03406365, 0.86373936, 1.34208835, 0.17277381, -0.25604465, -0.01137478, 0.35947248, -1.16306676, -1.48415467, -0.55550353, -0.89489301, 0.69315824, 1.39122322, 2.49402926, 0.47087818, 2.21859059, -0.04094598, -0.99877343, -0.79492149, -1.89477116, -1.43949239, -1.28168133, 0.45334232, 0.63770567, 1.19734629, 1.43219662, 2.12367306, -0.12322382, -0.03191014, 1.12164915, -0.30415023, -0.96659623, 1.51424482, 0.10537468, -0.37200959, 1.17823658, 0.31951541, -0.38950362, 0.05797512, 0.40091369, 0.75460205, -0.6293012, 1.03765105, 0.19644621, 0.24742472, -0.048953, -0.05036086, -1.67291093, -1.47894265, 0.73993955, -0.17106826, 0.68997245, -0.04175674, -0.47141506, -1.15926283, -0.06362598, -2.48271981, -1.29592079, -2.09476215, -0.1135624, 0.41134514, 1.93918511, -0.5250021, 0.84579518, 0.77597047, 0.33245888, 0.33367196, 0.53470905, -0.08054362, 0.2393}
#define FINAL_RLS_FILTER_W {-0.07973261, -0.41721576, 0.09297072, 0.04426754, 0.09051538, 0.02276504, 0.54175942, 0.12361774, 0.48683584, 0.29229921, -0.16090455, -0.09111357, -0.13366659, -0.09821963, -0.3025, 0.0}
#define RLS_ERROR {0.99726499, 0.9396903, 0.93623326, 0.93177918, 0.92971237, 0.92864321, 0.84284379, 0.83249113, 0.79890827, 0.74446457, 0.73671947, 0.73764171, 0.73907068, 0.70433776, 0.66591917, 0.67215288, 0.67078239, 0.62906764, 0.57622663, 0.47425008, 0.45559753, 0.33643786, 0.34326433, 0.33949394, 0.32034624, 0.30845521, 0.29753987, 0.30002587, 0.24684431, 0.22822409, 0.22059598, 0.22850683, 0.19549882, 0.19389456, 0.19994731, 0.19935399, 0.19882693, 0.19245958, 0.20121373, 0.20154632, 0.18834727, 0.18487805, 0.18251577, 0.17628743, 0.17875399, 0.18127419, 0.1710078, 0.1737303, 0.19558938, 0.17845935, 0.18432626, 0.18499848, 0.18447079, 0.18218468, 0.1829688, 0.18462933, 0.16542596, 0.16812265, 0.1552136, 0.12042027, 0.11859577, 0.11538965, 0.12113464, 0.11994053, 0.11058389, 0.10850916, 0.1124751, 0.11190688, 0.10644984, 0.10573318, 0.1060031, 0.10608915, 0.10638444, 0.10648925, 0.10655132, 0.10651321, 0.1078011, 0.1060264, 0.10598588, 0.1019892, 0.1019917, 0.10394281, 0.1039901, 0.10386766, 0.10247587, 0.10217785, 0.10208882, 0.10168952, 0.09976511, 0.10185556, 0.10107526, 0.09954322, 0.09917471, 0.09938139, 0.09700317, 0.09747736, 0.09200383, 0.09098431, 0.08752638, 0.08509155, 0.08363488, 0.08265594, 0.08289485, 0.07438441, 0.07458904, 0.07484423, 0.07633659, 0.08060987, 0.08053477, 0.08027184, 0.08104878, 0.0845418, 0.0847178, 0.08691286, 0.08547997, 0.0846382, 0.084308, 0.08470955, 0.08180803, 0.08008291, 0.0814833, 0.0817193, 0.08246194, 0.08229535, 0.0818886, 0.08089683, 0.08145262, 0.0819144, 0.081466, 0.08187976, 0.08172252, 0.08162213, 0.08220488, 0.08230012, 0.08285981, 0.08311374, 0.08262254, 0.0823457, 0.08232419, 0.08313604, 0.08305014, 0.08307875, 0.082675, 0.08129858, 0.0813183, 0.0802904, 0.07943505, 0.07959151, 0.07958057, 0.07660447, 0.07690152, 0.07681261, 0.07800318, 0.07560401, 0.07346475, 0.06933409, 0.06871249, 0.0687756, 0.06789864, 0.06797689, 0.06935838, 0.06934235, 0.06972861, 0.06872807, 0.06919684, 0.0694778, 0.06839359, 0.06832694, 0.06874059, 0.06744664, 0.06663982, 0.06501369, 0.06577323, 0.06553411, 0.06308066, 0.0637852, 0.06258865, 0.06220279, 0.06177327, 0.06040241, 0.05816357, 0.05827515, 0.05885458, 0.05916138, 0.05822044, 0.05848887, 0.05892931, 0.05872859, 0.05938129, 0.06166418, 0.06005871, 0.05964734, 0.05925514, 0.05589814, 0.05467159, 0.05388518, 0.055198, 0.05569285, 0.05635162, 0.05658718, 0.05660836, 0.05626019, 0.05550321, 0.05523889, 0.05431617, 0.05336723, 0.05363437, 0.05547189, 0.05579156, 0.05576615, 0.05600822, 0.0563715, 0.05604686, 0.05611411, 0.05600805, 0.05540077, 0.05492726, 0.05592254, 0.05584606, 0.05573173, 0.0564638, 0.05887669, 0.05859499, 0.0586734, 0.05834904, 0.06087685, 0.06017497, 0.05940648, 0.05940057, 0.05868745, 0.05907496, 0.05786161, 0.05576029, 0.05612938, 0.05532208, 0.05500038, 0.05612195, 0.0579326, 0.05770679, 0.05736838, 0.0571066, 0.05552656, 0.05539084, 0.05559786, 0.0557925, 0.05557636, 0.05554193, 0.05402585, 0.05449732, 0.05667148, 0.05801532, 0.05773961, 0.05698937, 0.05692186, 0.05721931, 0.0581}