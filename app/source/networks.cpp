#include "networks.hpp"

#include <array>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <format>
#include <sys/stat.h>

std::string networkPatchError;

// Sysmodule patches go to /luma/sysmodules/<id>.ips, Miiverse (JPN/USA/EUR) to /luma/titles/<id>/code.ips
static constexpr std::array<const char *, 5> SYSMODULE_IDS = {
	"0004013000003202", // friends
	"0004013000003802", // act
	"0004013000002902",
	"0004013000002E02",
	"0004013000002F02",
};
static constexpr std::array<const char *, 3> TITLE_IDS = {
	"000400300000BC02",
	"000400300000BD02",
	"000400300000BE02",
};

const char *networkName(Network network)
{
	switch (network) {
		case Network::Nintendo: return "Nintendo";
		case Network::Pretendo: return "Pretendo";
		default: return "Protarium";
	}
}

const char *networkDir(Network network)
{
	switch (network) {
		case Network::Nintendo: return "nintendo";
		case Network::Pretendo: return "pretendo";
		default: return "protarium";
	}
}

bool activeNetworkIsProtarium()
{
	char buf[16] = {0};
	FILE *file = std::fopen(NIMBUS_ACTIVE_NETWORK_PATH, "rb");
	if (!file) return false;
	std::fread(buf, 1, sizeof(buf) - 1, file);
	std::fclose(file);
	return std::strcmp(buf, "protarium") == 0;
}

static bool fail(const char *step, const std::string &path)
{
	networkPatchError = std::format("{}: {} (err {})", step, path, errno);
	return false;
}

static bool exists(const std::string &path)
{
	struct stat st;
	return stat(path.c_str(), &st) == 0;
}

// Copies through <dst>.nimbus.tmp and swaps it in, keeping <dst>.nimbus.old until the swap succeeded
static bool copyFile(const std::string &src, const std::string &dst)
{
	const std::string tmp = dst + ".nimbus.tmp";
	const std::string old = dst + ".nimbus.old";

	FILE *in = std::fopen(src.c_str(), "rb");
	if (!in) return fail("Read", src);

	FILE *out = std::fopen(tmp.c_str(), "wb");
	if (!out) {
		std::fclose(in);
		return fail("Write", tmp);
	}

	char buf[4096];
	size_t n;
	bool ok = true;
	while (ok && (n = std::fread(buf, 1, sizeof(buf), in)) > 0) {
		ok = std::fwrite(buf, 1, n, out) == n;
	}
	ok = ok && !std::ferror(in);
	std::fclose(in);
	ok = (std::fclose(out) == 0) && ok;
	if (!ok) {
		std::remove(tmp.c_str());
		return fail("Copy", dst);
	}

	std::remove(old.c_str());
	std::rename(dst.c_str(), old.c_str()); // fine if dst doesn't exist yet
	if (std::rename(tmp.c_str(), dst.c_str()) != 0) {
		std::rename(old.c_str(), dst.c_str());
		return fail("Rename", dst);
	}
	std::remove(old.c_str());
	return true;
}

static bool applyPatch(Network network, const std::string &variantDir, const char *id, const std::string &dst)
{
	if (network == Network::Nintendo) {
		std::remove(dst.c_str());
		return true;
	}

	const std::string src = variantDir + id + ".ips";
	if (!exists(src)) return fail("Missing", src);
	return copyFile(src, dst);
}

bool installNetworkPatches(Network network)
{
	networkPatchError.clear();

	for (const char *dir : {"/luma", "/luma/sysmodules", "/luma/titles"}) {
		if (mkdir(dir, 0777) != 0 && errno != EEXIST) return fail("Directory", dir);
	}

	const std::string variantDir = std::string(NIMBUS_VARIANTS_PATH) + networkDir(network) + "/";

	// The Miiverse patch reads its CA from /3ds/<pem>
	if (network != Network::Nintendo) {
		const char *pem = network == Network::Protarium ? "protaverse.pem" : "juxt-prod.pem";
		const std::string src = variantDir + pem;
		if (!exists(src)) return fail("Missing", src);
		if (!copyFile(src, std::string("/3ds/") + pem)) return false;
	}

	for (const char *id : SYSMODULE_IDS) {
		if (!applyPatch(network, variantDir, id, std::string("/luma/sysmodules/") + id + ".ips")) return false;
	}

	for (const char *id : TITLE_IDS) {
		const std::string dir = std::string("/luma/titles/") + id;
		if (mkdir(dir.c_str(), 0777) != 0 && errno != EEXIST) return fail("Directory", dir);
		if (!applyPatch(network, variantDir, id, dir + "/code.ips")) return false;
	}

	FILE *file = std::fopen(NIMBUS_ACTIVE_NETWORK_PATH, "wb");
	if (!file) return fail("Write", NIMBUS_ACTIVE_NETWORK_PATH);
	const char *name = networkDir(network);
	const bool ok = std::fwrite(name, 1, std::strlen(name), file) == std::strlen(name);
	std::fclose(file);
	return ok || fail("Write", NIMBUS_ACTIVE_NETWORK_PATH);
}
