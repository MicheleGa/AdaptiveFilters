#define LENGTH 16
#define N_SAMPLES 256
#define SNR 15
#define TOL 100

#define W_INIT {-0.25394635, 0.63817446, -0.21495415, 0.34535387, -0.44218667, 0.75764591, -0.35692718, 0.99660559, 0.01214659, 0.9062857, -1.13637043, -0.2135782, 1.25107562, 0.76210364, 0.665, 0.0}
#define W_CHECK {-0.09415729, 0.23661997, -0.07969991, 0.12804903, -0.16395234, 0.28091715, -0.13234014, 0.36951774, 0.00450367, 0.33602926, -0.42133923, -0.07918974, 0.4638692, 0.28256997, 0.2466, 0.0}
#define X {-0.84732888, 1.82391605, 0.05830419, 2.06933136, 2.06001103, 2.06175173, 0.5426901, 0.47947849, 1.81998585, 0.32622704, 0.65513134, -0.02049551, 0.15937038, 1.32379602, -0.85073453, 2.2729245, 2.33413976, -0.32168454, -0.5606229, 0.85309173, 1.88142326, -1.4466745, 0.21260053, 1.0916656, -0.29517062, -0.18527517, 0.83244039, -0.71937035, 0.21049882, 0.65519097, -0.54643671, -0.5656956, 0.72214762, 1.19627951, 0.22431935, 0.3372149, -0.82291912, 0.17444996, 0.08936597, -1.12746265, -1.27150758, -0.71340512, -1.09839414, -1.21744579, 0.84446501, 0.19598824, 0.11947412, -0.82736678, 1.31336027, -0.50791475, -0.8600035, -0.24294981, 0.72441998, 0.93705154, 0.73740024, -1.13729372, -0.95534631, -0.68039848, 0.25994375, -1.33937082, -1.07150137, -0.1649883, 1.04043261, -1.06526815, -1.20888324, 1.36904157, 0.33267749, 1.10728388, 0.24808828, 1.16952982, 0.10206558, -1.57251244, 0.20034784, 0.2487689, 0.95949756, 0.603656, -0.10954768, 0.41587684, 0.78687879, -0.72682891, 1.35713718, 0.37317413, 0.35601921, -0.99451219, 1.78161135, 0.23643665, -1.09722776, 0.3277223, -0.70350777, 1.90248953, -0.11774448, 0.28470299, 1.26711736, -0.00664711, -0.03637537, -0.2905537, 1.08965107, 0.14349784, -0.04569594, -0.91510178, -1.0193898, -1.16991716, -0.66441657, 0.66961625, -0.4763363, -0.04721531, -1.52407498, 0.36119441, -0.72717839, 0.49635728, 1.31824452, 0.53200381, 0.42064306, -0.04106565, 0.7379724, 1.19012285, -0.1633765, -1.17389872, -0.20236885, -0.39023073, -0.06699367, -0.34830402, 1.92649455, -0.58545593, -1.04439398, -0.85450531, -0.21163549, 0.2175909, 0.70451013, 0.12343116, 2.96588543, 0.04124313, 0.88637093, -1.23670369, 0.43442034, -1.26684066, -1.12007924, -0.16802477, -1.34683904, -0.84473134, -0.66392952, -0.63139282, -2.1236753, 0.22505883, 2.15587221, -0.81868147, 0.02641785, 1.20607058, -1.75817583, 0.40242785, 0.39498737, 1.64870966, 0.11301378, -0.9803362, -0.37250941, 0.71460314, -0.98124699, 0.78619985, 1.15001694, 1.69843857, -0.87744672, 0.46469294, 0.6022099, 0.22620988, 0.04024035, -0.6337143, 0.17527704, 0.64147226, -1.86002673, -0.49100912, -0.05646951, 1.53273043, 1.71210442, 0.77978132, 0.39153585, 1.42385526, -0.71276037, 0.25789058, -0.18328684, 1.25336788, 0.44376299, 0.37195008, -1.00439949, 0.97202763, -1.62876051, -0.05425015, -0.5789973, 0.6655161, -0.63223554, 0.79459989, -0.31479245, -0.81006041, -0.08206271, -1.03843901, -0.92087699, 0.7689881, -0.84313265, 1.57239951, -0.57191152, -0.37468493, -0.02484468, -1.74341728, -1.17051032, 0.48609512, -0.92064746, 0.57938401, -1.59879092, -0.45399475, -0.53377601, -1.09234883, -0.52839073, -0.23955729, -0.12272614, -1.54536526, 1.48859068, -0.51400183, 0.83945359, 0.90773701, 0.77393847, -1.38719183, -1.04171681, 0.88976444, 1.18769459, 1.9486018, -1.65816612, 1.78951614, 0.8181864, 0.98291733, -0.86447902, 0.35893051, 0.61335005, 1.33692024, 0.04422443, 2.82940289, -1.04336444, 1.06540018, -0.50977856, 1.85721783, 0.96359045, -0.72103178, -1.49931885, -0.32689449, 0.36903742, -0.18485981, 0.66711237, -0.30553207, -0.96769599, -1.98799382, 2.24282382, -1.36643545, 0.41176697, -2.70382892, 0.07246953, -0.28109977, -1.39130272, 0.5931}
#define INPUT {-0.11430069, -0.34928784, 0.496538, -0.4287973, 0.80973014, -0.30607264, 0.90318543, -0.81491425, 1.17601292, -0.13008698, 1.73352307, 0.21303199, 0.65786163, 0.55542093, 0.55695698, 1.03994238, 1.93218567, 2.9294265, 1.29426637, 0.25022337, 1.53149287, 1.46362432, 1.34756042, -0.1880723, 1.12879738, 0.89829019, -1.54188863, 1.879615, 1.19677094, 1.42237196, -0.88946516, 0.96385486, 0.83197491, -0.3490221, 0.28550827, 0.29401277, -0.09968179, 0.19162594, 0.38632659, -0.23874448, 0.60558457, 0.4715143, 0.17514726, -1.42695857, -0.4842265, 0.32582534, 0.78891196, 0.32470043, -1.58940644, 0.07872324, -0.3894021, 0.0239896, -1.31337535, 0.76439656, -1.51787258, -0.56859326, -0.93070309, 0.64928023, -0.776097, 0.56243315, 0.58614547, 0.57969196, -1.1142892, -0.4030406, -1.55774965, 0.43221627, 0.40459892, -0.08618185, -0.90203708, -1.16462844, 0.68645152, -0.71568372, -1.04963153, -0.53381344, 1.69142361, -0.52849103, -0.33052691, -0.18487677, -0.16823429, 1.31977764, -0.48244278, 2.25773192, 0.71775021, 0.50345306, -1.22289201, 0.85431144, 0.25215992, 1.57233598, -0.25548804, 1.24134108, -0.06995933, -0.15081603, 0.81065476, 0.49333356, 0.88073957, -1.70740531, 1.70981586, 0.72230302, 0.61593536, -0.18440231, -1.07077851, 1.11940474, -0.61465176, 0.69958318, 0.45618941, 0.60041133, -0.0198924, -1.48185815, 0.11451774, -0.34899928, 1.16846097, -0.55324254, 0.39149414, -2.20070804, -0.88245529, -0.38322742, 1.05747741, 0.26811152, -0.15573131, 0.64125591, -0.81952347, 0.28069227, 0.04484446, 1.6602916, -0.22223178, -0.38663363, -0.50272871, 1.46432992, -0.2393826, 0.45599831, -1.19875834, 1.01876074, -1.78200526, 0.08001391, 0.2025655, 1.98310413, -0.99775966, 0.98472665, -1.60003502, 1.18620889, -1.70692717, 0.39362964, 0.79749564, 0.41541444, 0.54803526, -0.47787796, 0.21296069, -1.30971814, -0.46709912, -1.05712598, -0.59336847, -0.69752898, 0.86002738, -0.45093106, -1.56320096, -1.51848644, 1.7982264, -1.09961903, 1.77279081, 0.23759687, 0.70535483, -1.64979672, 0.0896495, 0.72634896, 2.15326035, -1.06657753, 0.86564391, 0.08035115, 0.17753658, -0.94183131, 0.49301757, 1.05693594, 0.45644661, -0.32391303, 0.33737203, 0.20189153, 0.89793993, -1.01162889, 1.58262414, 0.7607492, 1.28684502, -1.17129786, -0.20081218, -0.59940401, 2.49542765, -0.1947315, 1.97915155, 0.29829933, 1.58722488, -1.46402258, 0.68130164, -1.01505785, 1.37004487, -0.8156679, 0.7401364, -0.28778511, 0.20866678, -0.96806224, -0.12835151, -0.90415003, -0.26398436, -0.12049662, 0.12101712, -0.97508355, 0.90093961, -1.53508547, 0.248217, -1.40537391, -0.74390233, -0.33040124, -0.21337888, -0.60005972, 0.98165386, -1.48845197, -1.66011424, -0.6217506, -1.34294666, 0.53061977, -1.85650821, 1.08145566, -1.6015686, -0.02499349, -1.21046298, 1.13370186, -0.11265227, -0.85152687, 0.55873252, -1.00679501, 1.25672561, -0.33763728, 3.32637555, -0.97705346, 0.43649444, -1.89780831, 3.28809979, 0.23985536, 1.09216941, -0.50521092, 2.84887793, 0.3006539, 1.84898073, -1.66230393, 1.61625892, -0.55845388, 1.65765004, 1.47379321, 1.03307671, 0.09516854, -2.05470855, 1.80785541, 0.36370156, 1.77651666, -2.83360313, 0.70268451, -2.56118514, 0.651}
#define FINAL_FILTER_W {-0.11817821, 0.26922095, -0.07301753, 0.12727048, -0.14543839, 0.31803102, -0.14793905, 0.38642791, 0.04440855, 0.3443593, -0.39122385, -0.07865312, 0.46111311, 0.27503714, 0.2132, 0.0}
#define ERROR {1.0085884, 0.98651245, 0.97086158, 0.96699739, 0.95803723, 0.9512258, 0.93614414, 0.922097, 0.89638775, 0.89512572, 0.83004218, 0.82763947, 0.82557783, 0.82475038, 0.82528923, 0.81791439, 0.80287757, 0.74653722, 0.74588103, 0.74016597, 0.73904363, 0.73766381, 0.72768452, 0.72535641, 0.72337338, 0.72254937, 0.65085822, 0.63009375, 0.61286185, 0.5855544, 0.55591022, 0.54897149, 0.53089339, 0.53148073, 0.52659165, 0.51883815, 0.51548188, 0.51530258, 0.51717035, 0.51909214, 0.48747142, 0.46626693, 0.46455458, 0.43696474, 0.43503227, 0.41818719, 0.40774065, 0.39111816, 0.37239025, 0.37282087, 0.3716708, 0.3333048, 0.33070309, 0.32868102, 0.27178819, 0.26703669, 0.24487782, 0.24322042, 0.24410818, 0.24273084, 0.23558374, 0.23021969, 0.22908864, 0.22398403, 0.19730667, 0.19194522, 0.19762778, 0.20075766, 0.19832647, 0.19364247, 0.19214154, 0.19025581, 0.19036291, 0.18724582, 0.18352136, 0.17033417, 0.17097945, 0.16878154, 0.16131578, 0.15062101, 0.14437801, 0.12849929, 0.12244836, 0.12522511, 0.12603124, 0.12955436, 0.1125161, 0.10931659, 0.11505747, 0.11489096, 0.12214141, 0.11550137, 0.11322537, 0.11639162, 0.11927272, 0.11948877, 0.11574303, 0.11397798, 0.11659797, 0.1065107, 0.10352218, 0.08697987, 0.08694346, 0.08681098, 0.09502894, 0.09615559, 0.09232773, 0.0956627, 0.10015604, 0.09293502, 0.09575987, 0.09578341, 0.09546827, 0.096801, 0.10326984, 0.10445256, 0.10918088, 0.09689518, 0.09918384, 0.11347923, 0.11460793, 0.11007232, 0.1134056, 0.11177363, 0.11921461, 0.1209921, 0.12185191, 0.12053114, 0.1275586, 0.12969165, 0.13080831, 0.13586169, 0.13453319, 0.13394479, 0.13444778, 0.13001253, 0.12731838, 0.13058331, 0.11914153, 0.11844413, 0.11701863, 0.11822853, 0.10412355, 0.09892853, 0.09885605, 0.09997775, 0.08745939, 0.09501696, 0.09059923, 0.09049519, 0.09878709, 0.09885305, 0.09345454, 0.09462963, 0.09622423, 0.09665611, 0.09674715, 0.09530725, 0.09149, 0.09245762, 0.09839842, 0.09878172, 0.09156868, 0.07889433, 0.08900523, 0.11530498, 0.11447855, 0.11855983, 0.11946119, 0.10591297, 0.11971592, 0.1195448, 0.11715973, 0.12652286, 0.12802571, 0.12976923, 0.12038752, 0.12598715, 0.1234025, 0.12407492, 0.13098416, 0.13255632, 0.13253158, 0.13085393, 0.13168437, 0.13152036, 0.13224953, 0.12510217, 0.12701836, 0.12749824, 0.11837801, 0.11729251, 0.11047123, 0.11066618, 0.10997169, 0.1098669, 0.10780767, 0.10767018, 0.10282364, 0.10297895, 0.10347703, 0.10103631, 0.09388579, 0.09387762, 0.09465955, 0.09484465, 0.08126808, 0.08482192, 0.09422534, 0.09596386, 0.09539834, 0.09346694, 0.09911842, 0.10573796, 0.10530385, 0.11269622, 0.12303563, 0.1248969, 0.12619457, 0.13158998, 0.13393457, 0.13425355, 0.13396001, 0.1322729, 0.12840365, 0.1240072, 0.116903, 0.11754456, 0.11508558, 0.11944809, 0.11927281, 0.11667875, 0.11321335, 0.1134305, 0.11243225, 0.11201587, 0.11037226, 0.11107074, 0.11097573, 0.11142169, 0.10819559, 0.1016287, 0.10038355, 0.10369269, 0.07818958, 0.07818229, 0.0786461, 0.07697628, 0.0776809, 0.07667209, 0.07872106, 0.07897636, 0.08224455, 0.08238785, 0.08362301, 0.0877}