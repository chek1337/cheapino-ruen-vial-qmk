# Docker-сборка прошивки cheapino RuEn.
# Использование:
#   make -f docker.mk build     — собрать Docker-образ
#   make -f docker.mk compile   — скомпилировать прошивку (по умолчанию cheapino:ruen)
#   make -f docker.mk clean     — удалить артефакты сборки (.build/)
#   make -f docker.mk shell     — интерактивный bash в контейнере

IMAGE ?= cheapino-qmk
KB    ?= cheapino
KM    ?= ruen

# -u: запускаем от UID хоста, чтобы .build/ был owned хост-пользователем
# -e HOME=/tmp: qmk_cli пишет конфиг в $HOME; /tmp всегда писабельный
DOCKER_RUN = docker run --rm \
	-u $(shell id -u):$(shell id -g) \
	-e HOME=/tmp \
	-v "$(CURDIR)":/qmk -w /qmk $(IMAGE)

.PHONY: build compile clean shell

build:
	docker build -t $(IMAGE) .

compile:
	$(DOCKER_RUN) qmk compile -kb $(KB) -km $(KM)

clean:
	$(DOCKER_RUN) rm -rf .build

shell:
	docker run --rm -it \
		-u $(shell id -u):$(shell id -g) \
		-e HOME=/tmp \
		-v "$(CURDIR)":/qmk -w /qmk $(IMAGE) bash
