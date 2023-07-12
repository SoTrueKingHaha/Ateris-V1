#include "basecombatweapon.h"
#include "../../SDK.h"

int CBaseCombatWeapon::GetBulletAmount( )
{
	auto v7 = *( DWORD * )( this + 2864 );
	auto v8 = *( DWORD * )( this + 2852 ) << 6;
	int bulletspershot = *( DWORD * )( v8 + v7 + 1788 );
	if ( bulletspershot >= 1 )
	{
		Utils::ATTRIB_HOOK_FLOAT( bulletspershot, "mult_bullets_per_shot", this, 0, true );
	}
	else
	{
		bulletspershot = 1;
	}
	return bulletspershot;
}

/*#include "../Draw/Draw.h"

#define singlecase(id, string) case id: { static CHudTexture* pTexture = g_Draw.GetIcon(string); return pTexture; }

#define rettexture(string) static CHudTexture* pTexture = g_Draw.GetIcon(string); return pTexture;

CHudTexture* CTFWeaponBase::GetWeaponIcon()
{
	switch (m_iItemDefinitionIndex()) {
	case 13:
	case 200:
	case 669:
	case 799:
	case 808:
	case 888:
	case 897:
	case 906:
	case 915:
	case 964:
	case 973:
	case 15002:
	case 15015:
	case 15021:
	case 15029:
	case 15036:
	case 15053:
	case 15065:
	case 15069:
	case 15106:
	case 15107:
	case 15108:
	case 15131:
	case 15151:
	case 15157: {
		rettexture("d_scattergun");
	}
	case 45:
	case 1078: {
		rettexture("d_force_a_nature");
	}

	singlecase(220, "d_shortstop");
	singlecase(448, "d_soda_popper");
	singlecase(772, "d_pep_brawlerblaster");
	singlecase(1103, "d_back_scatter");

	case 22:
	case 23:
	case 209:
	case 15013:
	case 15018:
	case 15035:
	case 15041:
	case 15046:
	case 15056:
	case 15060:
	case 15061:
	case 15100:
	case 15101:
	case 15102:
	case 15126:
	case 15148: {
		rettexture("d_pistol");
	}

	singlecase(46, "d_taunt_scout"); //Bonk has no icon but there is a taunt kill that says bonk so we'll use that
	singlecase(1145, "d_taunt_scout"); //Bonk has no icon but there is a taunt kill that says bonk so we'll use that
	singlecase(160, "d_maxgun");
	singlecase(294, "d_maxgun");
	singlecase(449, "d_the_winger");
	singlecase(773, "d_pep_pistol");
	singlecase(812, "d_guillotine");
	singlecase(833, "d_guillotine");
	singlecase(30666, "d_the_capper");
	singlecase(0, "d_bat");
	singlecase(190, "d_bat");
	singlecase(660, "d_bat");
	singlecase(44, "d_sandman");
	singlecase(221, "d_holymackerel");
	singlecase(999, "d_holymackerel");
	singlecase(317, "d_candycane");
	singlecase(325, "d_bostonbasher");
	singlecase(349, "d_lavabat");
	singlecase(355, "d_warfan");
	singlecase(450, "d_atomizer");
	singlecase(452, "d_scout_sword");
	singlecase(474, "d_nonnonviolent_protest");
	singlecase(572, "d_unarmed_combat");
	singlecase(648, "d_wrap_assassin");
	singlecase(939, "d_skull");
	singlecase(880, "d_freedom_staff");
	singlecase(949, "d_memory_maker"); //dunno if this is right but i dont have the weapon so
	singlecase(1013, "d_ham_shank");
	singlecase(1123, "d_necro_smasher");
	singlecase(1127, "d_crossing_guard");//dunno if this is right but i dont have the weapon so
	singlecase(30667, "d_batsaber");
	singlecase(30758, "d_prinny_machete");


	case 18:
	case 205:
	case 800:
	case 809:
	case 889:
	case 898:
	case 907:
	case 916:
	case 965:
	case 974:
	case 15006:
	case 15014:
	case 15028:
	case 15043:
	case 15052:
	case 15057:
	case 15081:
	case 15104:
	case 15105:
	case 15130:
	case 15150:
	case 237:// rocket jumper :|
	case 658:
	{
		rettexture("d_tf_projectile_rocket");
	}

	singlecase(127, "d_rocketlauncher_directhit");
	singlecase(228, "d_blackbox");
	singlecase(1085, "d_blackbox");
	singlecase(414, "d_liberty_launcher");
	singlecase(441, "d_cow_mangler");
	singlecase(513, "d_quake_rl");
	singlecase(730, "d_dumpster_device");
	singlecase(1104, "d_airstrike");

	case 10:
	case 199:
	case 15003:
	case 15016:
	case 15044:
	case 15047:
	case 15085:
	case 15109:
	case 15132:
	case 15133:
	case 15152:
	case 1141:
	case 12:
	case 11:
	case 9: {
		rettexture("d_shotgun_soldier");
	}

singlecase(415, "d_reserve_shooter");
singlecase(442, "d_righteous_bison");
singlecase(1153, "d_panic_attack");
singlecase(6, "d_shovel");
singlecase(196, "d_shovel");
singlecase(128, "d_pickaxe");
singlecase(154, "d_paintrain");
singlecase(357, "d_demokatana");
singlecase(416, "d_market_gardener");
singlecase(447, "d_disciplinary_action");
singlecase(775, "d_pickaxe_escape");


	case 21:
	case 208:
	case 659:
	case 798:
	case 807:
	case 887:
	case 896:
	case 905:
	case 914:
	case 963:
	case 972:
	case 15005:
	case 15017:
	case 15030:
	case 15034:
	case 15049:
	case 15054:
	case 15066:
	case 15067:
	case 15068:
	case 15089:
	case 15090:
	case 15115:
	case 15141: {
		rettexture("d_flamethrower");
	}

	case 40:
	case 1146: {
		rettexture("d_backburner");
	}

	singlecase(215, "d_degreaser");
	singlecase(594, "d_phlogistinator");
	singlecase(741, "d_rainblower");
	singlecase(1178, "d_dragons_fury");

	case 39:
	case 1081: {
		rettexture("d_flaregun");
	}
	singlecase(740, "d_scorch_shot");
	singlecase(595, "d_manmelter");
	singlecase(351, "d_detonator");
	singlecase(1179, "d_rocketpack");

	case 2:
	case 192: {
		rettexture("d_fireaxe");
	}
	case 38:
	case 1000: {
		rettexture("d_axtinguisher");
	}
	singlecase(153, "d_sledgehammer");
	singlecase(214, "d_powerjack");
	singlecase(326, "d_back_scratcher");
	singlecase(348, "d_lava_axe");
	singlecase(457, "d_mailbox");
	singlecase(466, "d_the_maul");
	singlecase(593, "d_thirddegree");
	singlecase(739, "d_lollichop");
	case 813:
	case 834: {
		rettexture("d_annihilator");
	}
	singlecase(1181, "d_hot_hand");

	case 19:
	case 206:
	case 15077:
	case 15079:
	case 15091:
	case 15092:
	case 15116:
	case 15117:
	case 15142:
	case 15158:
	case 1007: {
		rettexture("d_tf_projectile_pipe");
	}

	singlecase(308, "d_loch_n_load");
	singlecase(996, "d_loose_cannon");
	singlecase(1151, "d_iron_bomber");

	case 20:
	case 207:
	case 661:
	case 797:
	case 806:
	case 886:
	case 895:
	case 904:
	case 913:
	case 962:
	case 971:
	case 265:
	case 15009:
	case 15012:
	case 15024:
	case 15038:
	case 15045:
	case 15048:
	case 15082:
	case 15083:
	case 15084:
	case 15113:
	case 15137:
	case 15138:
	case 15155: {
		rettexture("d_tf_projectile_pipe_remote");
	}

	singlecase(130, "d_sticky_resistance");
	singlecase(131, "d_demoshield");
	singlecase(1144, "d_demoshield");
	singlecase(1099, "d_tide_turner");
	singlecase(406, "d_splendid_screen");
	singlecase(1150, "d_quickiebomb_launcher");

	case 1:
	case 191: {
		rettexture("d_bottle");
	}

	singlecase(132, "d_sword");
	singlecase(1082, "d_sword");
	singlecase(172, "d_battleaxe");
	singlecase(266, "d_headtaker");
	singlecase(307, "d_ullapool_caber_explosion");
	singlecase(327, "d_claidheamohmor");
	singlecase(404, "d_persian_persuader");
	singlecase(482, "d_nessieclub");
	singlecase(609, "d_scotland_shard");

	case 15:
	case 202:
	case 654:
	case 793:
	case 802:
	case 41:
	case 312:
	case 424:
	case 882:
	case 891:
	case 900:
	case 909:
	case 958:
	case 967:
	case 15004:
	case 15020:
	case 15026:
	case 15031:
	case 15040:
	case 15055:
	case 15086:
	case 15087:
	case 15088:
	case 15098:
	case 15099:
	case 15123:
	case 15124:
	case 15125:
	case 15147: {
		rettexture("d_minigun");
	}
	singlecase(811, "d_long_heatmaker");
	singlecase(832, "d_long_heatmaker");
	singlecase(850, "d_iron_curtain");
	singlecase(298, "d_iron_curtain");

	singlecase(425, "d_family_business");

	case 5:
	case 195: {
		rettexture("d_fists");
	}
	singlecase(43, "d_gloves");
	singlecase(239, "d_gloves_running_urgently");
	singlecase(1084, "d_gloves_running_urgently");
	singlecase(1184, "d_gloves_running_urgently");
	singlecase(310, "d_warrior_spirit");
	singlecase(331, "d_steel_fists");
	singlecase(426, "d_eviction_notice");
	singlecase(587, "d_apocofists");
	singlecase(656, "d_holiday_punch");
	singlecase(1100, "d_bread_bite");

	singlecase(141, "d_frontier_justice");
	singlecase(1004, "d_frontier_justice");
	singlecase(527, "d_widowmaker");
	singlecase(588, "d_pomson");
	singlecase(997, "d_rescue_ranger");
	singlecase(140, "d_wrangler_kill");
	singlecase(1086, "d_wrangler_kill");
	singlecase(528, "d_short_circuit");
	singlecase(30668, "d_giger_counter");

	case 7:
	case 197:
	case 662:
	case 795:
	case 804:
	case 884:
	case 893:
	case 902:
	case 911:
	case 960:
	case 969:
	case 15073:
	case 15074:
	case 15075:
	case 15139:
	case 15140:
	case 15114:
	case 15156:
	case 169: {
		rettexture("d_wrench");
	}

	singlecase(142, "d_robot_arm");
	singlecase(155, "d_southern_hospitality");
	singlecase(329, "d_wrench_jag");
	singlecase(589, "d_eureka_effect");

	case 17:
	case 204: {
		rettexture("d_syringegun_medic");
	}
	singlecase(36, "d_blutsauger");
	case 305:
	case 1079: {
		rettexture("d_crusaders_crossbow");
	}
	singlecase(412, "d_proto_syringe");

	case 8:
	case 198:
	case 1143: {
		rettexture("d_bonesaw");
	}

	singlecase(37, "d_ubersaw");
	singlecase(1003, "d_ubersaw");

	singlecase(173, "d_battleneedle");
	singlecase(304, "d_amputator");
	singlecase(413, "d_solemnvow");

	case 14:
	case 201:
	case 664:
	case 792:
	case 801:
	case 881:
	case 890:
	case 899:
	case 908:
	case 957:
	case 966:
	case 15000:
	case 15007:
	case 15019:
	case 15023:
	case 15033:
	case 15059:
	case 15070:
	case 15071:
	case 15072:
	case 15111:
	case 15112:
	case 15135:
	case 15136:
	case 851: {
		rettexture("d_sniperrifle");
	}

	singlecase(56, "d_tf_projectile_arrow");
	singlecase(1005, "d_tf_projectile_arrow");
	singlecase(1092, "d_tf_projectile_arrow");

	singlecase(30665, "d_shooting_star");
	singlecase(1098, "d_the_classic");
	singlecase(752, "d_pro_rifle");
	singlecase(526, "d_machina");
	singlecase(402, "d_bazaar_bargain");
	singlecase(230, "d_sydney_sleeper");

	case 16:
	case 203:
	case 1149:
	case 15001:
	case 15022:
	case 15032:
	case 15037:
	case 15058:
	case 15076:
	case 15110:
	case 15134:
	case 15153: {
		rettexture("d_smg");
	}
	singlecase(3, "d_club");
	singlecase(193, "d_club");

	singlecase(171, "d_tribalkukri");
	singlecase(232, "d_bushwacka");
	singlecase(401, "d_shahanshah");


	case 24:
	case 210:
	case 1142:
	case 15011:
	case 15027:
	case 15042:
	case 15051:
	case 15062:
	case 15063:
	case 15064:
	case 15103:
	case 15128:
	case 15129:
	case 15149: {
		rettexture("d_revolver");
	}

	singlecase(61, "d_ambassador");
	singlecase(1006, "d_ambassador");

	singlecase(161, "d_samrevolver");

	singlecase(224, "d_letranger");
	singlecase(460, "d_enforcer");
	singlecase(525, "d_diamondback");

	singlecase(735, "d_psapper");
	singlecase(736, "d_psapper");
	singlecase(1080, "d_psapper");

	singlecase(810, "d_recorder");
	singlecase(831, "d_recorder");

	singlecase(933, "d_psapper"); // :(
	singlecase(1102, "d_psapper"); // :(

	singlecase(225, "d_eternal_reward");
	singlecase(356, "d_kunai");
	singlecase(461, "d_big_earner");
	singlecase(574, "d_voodoo_pin");
	singlecase(638, "d_sharp_dresser");
	singlecase(649, "d_spy_cicle");

	case 4:
	case 194:
	case 665:
	case 794:
	case 803:
	case 883:
	case 892:
	case 901:
	case 910:
	case 959:
	case 968:
	case 15094:
	case 15095:
	case 15096:
	case 15118:
	case 15119:
	case 15143:
	case 15144:
	case 727: // :(
	{
		rettexture("d_knife");
	}

	singlecase(27, "hud_spy_disguise_menu_icon");

	singlecase(264, "d_fryingpan");
	singlecase(1071, "d_fryingpan");

	default:
		static CHudTexture* pTexture = g_Draw.GetIcon("d_skull");
		return pTexture;
	}
}*/
/*#define singlecase(id, name) case id: return name;

std::wstring CTFWeaponBase::GetWeaponName( )
{
	switch ( m_iItemDefinitionIndex( ) ) {
		case 13:
		case 200:
		case 669:
		case 799:
		case 808:
		case 888:
		case 897:
		case 906:
		case 915:
		case 964:
		case 973:
		case 15002:
		case 15015:
		case 15021:
		case 15029:
		case 15036:
		case 15053:
		case 15065:
		case 15069:
		case 15106:
		case 15107:
		case 15108:
		case 15131:
		case 15151:
		case 15157: {
			return L"SCATTERGUN";
		}
		case 45:
		case 1078: {
			return L"FORCE-A-NATURE";
		}

		case 220: return L"SHORTSTOP";
		case 448: return L"SODA POPPER";
		case 772: return L"BABY FACES BLASTER";
		case 1103: return L"BACK SCATTER";

		case 22:
		case 23:
		case 209:
		case 15013:
		case 15018:
		case 15035:
		case 15041:
		case 15046:
		case 15056:
		case 15060:
		case 15061:
		case 15100:
		case 15101:
		case 15102:
		case 15126:
		case 15148: {
			return L"PISTOL";
		}

		case 46: return L"BONK"; //Bonk has no icon but there is a taunt kill that says bonk so we'll use that
		case 1145: return L"BONK"; //Bonk has no icon but there is a taunt kill that says bonk so we'll use that
		singlecase( 160, L"maxgun" );
		singlecase( 294, L"maxgun" );
		singlecase( 449, L"the_winger" );
		singlecase( 773, L"pep_pistol" );
		singlecase( 812, L"guillotine" );
		singlecase( 833, L"guillotine" );
		singlecase( 30666, L"the_capper" );
		singlecase( 0, L"bat" );
		singlecase( 190, L"bat" );
		singlecase( 660, L"bat" );
		singlecase( 44, L"sandman" );
		singlecase( 221, L"holymackerel" );
		singlecase( 999, L"holymackerel" );
		singlecase( 317, L"candycane" );
		singlecase( 325, L"bostonbasher" );
		singlecase( 349, L"lavabat" );
		singlecase( 355, L"warfan" );
		singlecase( 450, L"atomizer" );
		singlecase( 452, L"scout_sword" );
		singlecase( 474, L"nonnonviolent_protest" );
		singlecase( 572, L"unarmed_combat" );
		singlecase( 648, L"wrap_assassin" );
		singlecase( 939, L"skull" );
		singlecase( 880, L"freedom_staff" );
		singlecase( 949, L"memory_maker" ); //dunno if this is right but i dont have the weapon so
		singlecase( 1013, L"ham_shank" );
		singlecase( 1123, L"necro_smasher" );
		singlecase( 1127, L"crossing_guard" );//dunno if this is right but i dont have the weapon so
		singlecase( 30667, L"batsaber" );
		singlecase( 30758, L"prinny_machete" );


		case 18:
		case 205:
		case 800:
		case 809:
		case 889:
		case 898:
		case 907:
		case 916:
		case 965:
		case 974:
		case 15006:
		case 15014:
		case 15028:
		case 15043:
		case 15052:
		case 15057:
		case 15081:
		case 15104:
		case 15105:
		case 15130:
		case 15150:
		case 237:// rocket jumper :|
		case 658:
		{
			return L"tf_projectile_rocket";
		}

		singlecase( 127, L"rocketlauncher_directhit" );
		singlecase( 228, L"blackbox" );
		singlecase( 1085, L"blackbox" );
		singlecase( 414, L"liberty_launcher" );
		singlecase( 441, L"cow_mangler" );
		singlecase( 513, L"quake_rl" );
		singlecase( 730, L"dumpster_device" );
		singlecase( 1104, L"airstrike" );

		case 10:
		case 199:
		case 15003:
		case 15016:
		case 15044:
		case 15047:
		case 15085:
		case 15109:
		case 15132:
		case 15133:
		case 15152:
		case 1141:
		case 12:
		case 11:
		case 9: {
			return L"shotgun_soldier" ;
		}

		singlecase( 415, L"reserve_shooter" );
		singlecase( 442, L"righteous_bison" );
		singlecase( 1153, L"panic_attack" );
		singlecase( 6, L"shovel" );
		singlecase( 196, L"shovel" );
		singlecase( 128, L"pickaxe" );
		singlecase( 154, L"paintrain" );
		singlecase( 357, L"demokatana" );
		singlecase( 416, L"market_gardener" );
		singlecase( 447, L"disciplinary_action" );
		singlecase( 775, L"pickaxe_escape" );


		case 21:
		case 208:
		case 659:
		case 798:
		case 807:
		case 887:
		case 896:
		case 905:
		case 914:
		case 963:
		case 972:
		case 15005:
		case 15017:
		case 15030:
		case 15034:
		case 15049:
		case 15054:
		case 15066:
		case 15067:
		case 15068:
		case 15089:
		case 15090:
		case 15115:
		case 15141: {
			return L"flamethrower";
		}

		case 40:
		case 1146: {
			return L"backburner";
		}

		singlecase( 215, L"degreaser" );
		singlecase( 594, L"phlogistinator" );
		singlecase( 741, L"rainblower" );
		singlecase( 1178, L"dragons_fury" );

		case 39:
		case 1081: {
			return L"flaregun";
		}
		singlecase( 740, L"scorch_shot" );
		singlecase( 595, L"manmelter" );
		singlecase( 351, L"detonator" );
		singlecase( 1179, L"rocketpack" );

		case 2:
		case 192: {
			return L"fireaxe";
		}
		case 38:
		case 1000: {
			return L"axtinguisher";
		}
		singlecase( 153, L"sledgehammer" );
		singlecase( 214, L"powerjack" );
		singlecase( 326, L"back_scratcher" );
		singlecase( 348, L"lava_axe" );
		singlecase( 457, L"mailbox" );
		singlecase( 466, L"the_maul" );
		singlecase( 593, L"thirddegree" );
		singlecase( 739, L"lollichop" );
		case 813:
		case 834: {
			return L"annihilator";
		}
		singlecase( 1181, L"hot_hand" );

		case 19:
		case 206:
		case 15077:
		case 15079:
		case 15091:
		case 15092:
		case 15116:
		case 15117:
		case 15142:
		case 15158:
		case 1007: {
			return L"tf_projectile_pipe";
		}

		singlecase( 308, L"loch_n_load" );
		singlecase( 996, L"loose_cannon" );
		singlecase( 1151, L"iron_bomber" );

		case 20:
		case 207:
		case 661:
		case 797:
		case 806:
		case 886:
		case 895:
		case 904:
		case 913:
		case 962:
		case 971:
		case 265:
		case 15009:
		case 15012:
		case 15024:
		case 15038:
		case 15045:
		case 15048:
		case 15082:
		case 15083:
		case 15084:
		case 15113:
		case 15137:
		case 15138:
		case 15155: {
			return L"tf_projectile_pipe_remote";
		}

		singlecase( 130, L"sticky_resistance" );
		singlecase( 131, L"demoshield" );
		singlecase( 1144, L"demoshield" );
		singlecase( 1099, L"tide_turner" );
		singlecase( 406, L"splendid_screen" );
		singlecase( 1150, L"quickiebomb_launcher" );

		case 1:
		case 191: {
			return L"bottle";
		}

		singlecase( 132, L"sword" );
		singlecase( 1082, L"sword" );
		singlecase( 172, L"battleaxe" );
		singlecase( 266, L"headtaker" );
		singlecase( 307, L"ullapool_caber_explosion" );
		singlecase( 327, L"claidheamohmor" );
		singlecase( 404, L"persian_persuader" );
		singlecase( 482, L"nessieclub" );
		singlecase( 609, L"scotland_shard" );

		case 15:
		case 202:
		case 654:
		case 793:
		case 802:
		case 41:
		case 312:
		case 424:
		case 882:
		case 891:
		case 900:
		case 909:
		case 958:
		case 967:
		case 15004:
		case 15020:
		case 15026:
		case 15031:
		case 15040:
		case 15055:
		case 15086:
		case 15087:
		case 15088:
		case 15098:
		case 15099:
		case 15123:
		case 15124:
		case 15125:
		case 15147: {
			return L"minigun";
		}
		singlecase( 811, L"long_heatmaker" );
		singlecase( 832, L"long_heatmaker" );
		singlecase( 850, L"iron_curtain" );
		singlecase( 298, L"iron_curtain" );

		singlecase( 425, L"family_business" );

		case 5:
		case 195: {
			return L"fists";
		}
		singlecase( 43, L"gloves" );
		singlecase( 239, L"gloves_running_urgently" );
		singlecase( 1084, L"gloves_running_urgently" );
		singlecase( 1184, L"gloves_running_urgently" );
		singlecase( 310, L"warrior_spirit" );
		singlecase( 331, L"steel_fists" );
		singlecase( 426, L"eviction_notice" );
		singlecase( 587, L"apocofists" );
		singlecase( 656, L"holiday_punch" );
		singlecase( 1100, L"bread_bite" );

		singlecase( 141, L"frontier_justice" );
		singlecase( 1004, L"frontier_justice" );
		singlecase( 527, L"widowmaker" );
		singlecase( 588, L"pomson" );
		singlecase( 997, L"rescue_ranger" );
		singlecase( 140, L"wrangler_kill" );
		singlecase( 1086, L"wrangler_kill" );
		singlecase( 528, L"short_circuit" );
		singlecase( 30668, L"giger_counter" );

		case 7:
		case 197:
		case 662:
		case 795:
		case 804:
		case 884:
		case 893:
		case 902:
		case 911:
		case 960:
		case 969:
		case 15073:
		case 15074:
		case 15075:
		case 15139:
		case 15140:
		case 15114:
		case 15156:
		case 169: {
			return L"wrench";
		}

		singlecase( 142, L"robot_arm" );
		singlecase( 155, L"southern_hospitality" );
		singlecase( 329, L"wrench_jag" );
		singlecase( 589, L"eureka_effect" );

		case 17:
		case 204: {
			return L"syringegun_medic";
		}
		singlecase( 36, L"blutsauger" );
		case 305:
		case 1079: {
			return L"crusaders_crossbow";
		}
		singlecase( 412, L"proto_syringe" );

		case 8:
		case 198:
		case 1143: {
			return L"bonesaw";
		}

		singlecase( 37, L"ubersaw" );
		singlecase( 1003, L"ubersaw" );

		singlecase( 173, L"battleneedle" );
		singlecase( 304, L"amputator" );
		singlecase( 413, L"solemnvow" );

		case 14:
		case 201:
		case 664:
		case 792:
		case 801:
		case 881:
		case 890:
		case 899:
		case 908:
		case 957:
		case 966:
		case 15000:
		case 15007:
		case 15019:
		case 15023:
		case 15033:
		case 15059:
		case 15070:
		case 15071:
		case 15072:
		case 15111:
		case 15112:
		case 15135:
		case 15136:
		case 851: {
			return L"sniperrifle";
		}

		singlecase( 56, L"tf_projectile_arrow" );
		singlecase( 1005, L"tf_projectile_arrow" );
		singlecase( 1092, L"tf_projectile_arrow" );

		singlecase( 30665, L"shooting_star" );
		singlecase( 1098, L"the_classic" );
		singlecase( 752, L"pro_rifle" );
		singlecase( 526, L"machina" );
		singlecase( 402, L"bazaar_bargain" );
		singlecase( 230, L"sydney_sleeper" );

		case 16:
		case 203:
		case 1149:
		case 15001:
		case 15022:
		case 15032:
		case 15037:
		case 15058:
		case 15076:
		case 15110:
		case 15134:
		case 15153: {
			return L"smg";
		}
		singlecase( 3, L"club" );
		singlecase( 193, L"club" );

		singlecase( 171, L"tribalkukri" );
		singlecase( 232, L"bushwacka" );
		singlecase( 401, L"shahanshah" );


		case 24:
		case 210:
		case 1142:
		case 15011:
		case 15027:
		case 15042:
		case 15051:
		case 15062:
		case 15063:
		case 15064:
		case 15103:
		case 15128:
		case 15129:
		case 15149: {
			return L"revolver";
		}

		singlecase( 61, L"ambassador" );
		singlecase( 1006, L"ambassador" );

		singlecase( 161, L"samrevolver" );

		singlecase( 224, L"letranger" );
		singlecase( 460, L"enforcer" );
		singlecase( 525, L"diamondback" );

		singlecase( 735, L"psapper" );
		singlecase( 736, L"psapper" );
		singlecase( 1080, L"psapper" );

		singlecase( 810, L"recorder" );
		singlecase( 831, L"recorder" );

		singlecase( 933, L"psapper" ); // :(
		singlecase( 1102, L"psapper" ); // :(

		singlecase( 225, L"eternal_reward" );
		singlecase( 356, L"kunai" );
		singlecase( 461, L"big_earner" );
		singlecase( 574, L"voodoo_pin" );
		singlecase( 638, L"sharp_dresser" );
		singlecase( 649, L"spy_cicle" );

		case 4:
		case 194:
		case 665:
		case 794:
		case 803:
		case 883:
		case 892:
		case 901:
		case 910:
		case 959:
		case 968:
		case 15094:
		case 15095:
		case 15096:
		case 15118:
		case 15119:
		case 15143:
		case 15144:
		case 727: // :(
		{
			return L"knife";
		}

		singlecase( 27, L"hud_spy_disguise_menu_icon" );

		singlecase( 264, L"fryingpan" );
		singlecase( 1071, L"fryingpan" );

		default: return L"NONE";
	}
}*/