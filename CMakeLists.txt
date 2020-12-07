idf_component_register(
	SRCS "x_values_convert.c" "x_values_to_string.c"
	INCLUDE_DIRS "."
	PRIV_INCLUDE_DIRS
	REQUIRES statistics
	PRIV_REQUIRES common printfx
)
