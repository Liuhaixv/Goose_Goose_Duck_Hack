// !! means that the information has not been collected yet.
// Outdated. New roles added.
#pragma once

namespace roles {

	namespace goose {

		constexpr int Goose = 0x1;
		constexpr int Birdwatcher = 0x16;
		constexpr int Canadian = 0xD;
		constexpr int Detective = 0x14;
		constexpr int Gravy = 0x4;
		constexpr int Mechanic = 0xAF5;  // !!
		constexpr int Medium = 0x7;
		constexpr int Sherrif = 0xB;
		constexpr int Snoop = 0x5;
		constexpr int Tech = 0x6;
		constexpr int Vigilante = 0x8;
		constexpr int Celebrity = 0x1F;
		constexpr int Bodyguard = 0xCFC;  // !!
		constexpr int Politician = 0x1C;
		constexpr int Locksmith = 0xDBC;  // !!
		constexpr int Mortician = 0x1E;
		constexpr int Mimic = 0x13;
		constexpr int Lover = 0xF; // goose

	}

	namespace duck {

		constexpr int Assassin = 0x17;
		constexpr int Duck = 0x2;
		constexpr int Cannibal = 0x9;
		constexpr int Professional = 0x11;
		constexpr int Silencer = 0xC;
		constexpr int Spy = 0x12;
		constexpr int Morphling = 0xA;
		constexpr int Hitman = 0xDEC;  // !!
		constexpr int Snitch = 0x1B;
		constexpr int Party = 0xEAC;  // !!
		constexpr int Demolitionist = 0x21;
		constexpr int Lover = 0xE; // Duck

	}

	namespace Neutrality {

		constexpr int Pigeon = 0x15;
		constexpr int Vulture = 0x10;
		constexpr int Falcon = 0x18;
		constexpr int Dodo = 0x3;
		constexpr int NoRole = -1;

	}

	namespace halloween {

		constexpr int Villager = 0xEDF; // !!
		constexpr int Vampire = 0xEFF; // !!
		constexpr int Thrall = 0xFAF; // !!

	}

}

namespace colors { // enum must look cool~
	constexpr int red = 0;
	constexpr int blue = 1;
	constexpr int yellow = 2;
	constexpr int green = 3;
	constexpr int powerderPink = 4;
	constexpr int orange = 5;
	constexpr int paleBrown = 6;
	constexpr int white = 7;
	constexpr int darkGray = 8;
	constexpr int purple = 9;
	constexpr int lime = 10;
	constexpr int skyBlue = 11;
	constexpr int pink = 12;
	constexpr int gray = 13;
	constexpr int brown = 14;
	constexpr int darkNavy = 15;
	constexpr int olive = 16;
	constexpr int black = 17;
	constexpr int wineRed = 18;
	constexpr int beige = 19;
}

enum gameStateCode
{
	InLobby,
	Drafting,
	InGame,
	Opening, // Before Discussion
	Discussion,
	Voting,
	Waiting,
	Proceeding
};

enum gameRoleId
{
	None,
	Goose,
	Duck,
	Dodo,
	Bounty,
	Mechanic,
	Technician,
	Medium,
	Vigilante,
	Cannibal,
	Morphling,
	Sheriff,
	Silencer,
	Canadian,
	LoverDuck,
	LoverGoose,
	Vulture,
	Professional,
	Spy,
	Mimic,
	Detective,
	Pigeon,
	Birdwatcher,
	Assassin,
	Falcon,
	Hitman,
	Bodyguard,
	Snitch,
	Politician,
	Locksmith,
	Mortician,
	Celebrity,
	Party,
	Demolitionist,
	DuelingDodo,
	GHGoose,
	GHDuck,
	GHBounty,
	HNSGoose,
	HNSDuck,
	HNSBounty,
	DNDDuck,
	DNDFalcon,
	DNDVulture,
	DNDMorphling,
	FPGoose,
	ExploreGoose,
	TTVampire,
	TTPeasant,
	TTThrall,
	Spectator,
	IdentityThief,
	Adventurer,
	Avenger,
	Ninja,
	Undertaker,
	Snoop,
	Esper,
	Invisibility,
	Astral,
	Pelican,
	TTEThrall,
	TTMummy,
	SerialKiller,
	Engineer,
	Warlock,
	StreetUrchin,
	Tracker
};

/*
*  Managers.PlayerManagers.PlayerRolesManager

public enum GCKOOHMKNFO : short
*/