# Цель по умолчанию
all:
	docker-compose up --build -d


# Цель для запуска e2e тестов
# Цель для запуска e2e тестов
e2e-test:
	powershell -ExecutionPolicy Bypass -File ./tests/e2e/e2e-test.ps1

# ./tests/e2e/e2e-test-a.sh
# ./tests/e2e/e2e-test-b.sh



# Цель для очистки всех сгенерированных файлов
clean:
	docker-compose stop
