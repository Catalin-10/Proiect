FROM gcc:13

WORKDIR /app

COPY . .

RUN g++ -std=c++17 Calendar.cpp -o Calendarexemplu

ENTRYPOINT ["./Calendarexemplu"]
