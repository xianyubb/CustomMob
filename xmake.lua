add_rules("mode.debug", "mode.release")

add_repositories("liteldev-repo https://github.com/LiteLDev/xmake-repo.git")


add_requires(
    "levilamina 1.0.0-rc.3",
    "levibuildscript 0.3.0"
)

if not has_config("vs_runtime") then
    set_runtimes("MD")
end

target("CustomMob")
    add_cxflags(
        "/EHa",
        "/utf-8",
        "/W4",
        "/w44265",
        "/w44289",
        "/w44296",
        "/w45263",
        "/w44738",
        "/w45204"
    )
    add_defines(
        "NOMINMAX", 
        "UNICODE", 
        "_HAS_CXX17",
        "_HAS_CXX20"
    )
    add_files("src/**.cpp")
    add_includedirs("src")
    add_packages("levilamina")
    add_rules("@levibuildscript/linkrule")
    add_rules("@levibuildscript/modpacker")


    set_exceptions("none")
    set_kind("shared")
    set_languages("c++20")
    set_symbols("debug")