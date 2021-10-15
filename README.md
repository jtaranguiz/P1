# P1 Sistemas Operativos y Redes

# Grupo Sintax Error

Integrantes:

Fernando Alba: 17636736

Jorge Schenke: 17641624

Cristóbal Vial: 17638526

Diego García 1764240j

José Tomás Aránguiz: 17639565

Para llevar a cabo el desarrollo de la API tuvimos que pensar en cómo ibamos a separar los distintos desafíos que involucraba, junto con la mejor forma de ir actualizando los avances al resto del equipo. Esto que si bien parece fácil el separar por funciones, no lo es en la práctica, ya que distintas partes del proyecto involucraban distintos conocimientos del funcionamiento de la memoria. Por lo que en un principio el principal desafío de diseño fue el ponernos de acuerdo los integrantes del grupo en cual sería la mejor forma de organizar la memoria en las distintas estructuras que fuimos desarrollando. Así fue como armamos una estructura para los distintos procesos, los cuales contenian otras estructuras tales como las subentras y la tabla de pagina y que sería contenida por la tabla de procesos.

Luego, teniendo semi-armada la estructura también teníamos distintas interpretaciones de lo que sería la  mejor forma de desarrollar cada una de las funciones, pero siempre se pudo llegar a una solución. Nos separamos en grupos de 3 y 2, informando todos los cambios que se fueron realizando. Creemos que una vez realizada la asignación de la memoria a las distintas estructuras de nuestro programa, el desarrollo de las funciones de nuestra API fueron desarrolladas bajo una linea similar, por lo que en cuanto a diseño, no tuvimos que hacer grandes cambios cuando llego el momento de integrar las distintas partes.

Por último, en cuanto a supuestos que se realizaron en el desarrollo de nuestra API, está el asumir que la funcion cr_close se trataba simplemente de desapuntar el archivo dentro de nuestro programa. Además, se asumió que los procesos vacíos tenían id=0, lo cual se manejó para no perjudicar el caso de un posible proceso con contenido que tuviera el mismo id 0. Por último, eliminar una entrada se consideró como el cambiar su validez a 0.
