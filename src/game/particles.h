Renderer::ParticleSystem rightLaser;
Renderer::ParticleSystem leftLaser;
Renderer::ParticleSystem middleLaser;

Renderer::ParticleSystem hitEnemy;
Renderer::ParticleSystem hitCog;

void init_laser_particles() {
    rightLaser = Renderer::create_particle_system(5, 1000, V2(0, 0));
    rightLaser.one_size = true;
    rightLaser.alive_time = {0.2, 0.5};
    rightLaser.position_x = {0.2, 0.3};
    rightLaser.position_y = {-0.15, -0.1};
    rightLaser.velocity_dir = {0, 2*PI};
    rightLaser.spawn_size = {0.01, 0.02};
    rightLaser.velocity = {0.5, 1};

    leftLaser = Renderer::create_particle_system(5, 1000, V2(0, 0));
    leftLaser.one_size = true;
    leftLaser.alive_time = {0.2, 0.5};
    leftLaser.position_x = {-0.3, -0.2};
    leftLaser.position_y = {-0.15, -0.1};
    leftLaser.velocity_dir = {0, 2*PI};
    leftLaser.spawn_size = {0.01, 0.02};
    leftLaser.velocity = {0.5, 1};

    middleLaser = Renderer::create_particle_system(5, 1000, V2(0, 0));
    middleLaser.one_size = true;
    middleLaser.alive_time = {0.2, 0.5};
    middleLaser.position_x = {-0.1, 0.1};
    middleLaser.position_y = {0.2, 0.3};
    middleLaser.velocity_dir = {0, 2*PI};
    middleLaser.spawn_size = {0.02, 0.03};
    middleLaser.velocity = {0.5, 1};
}

void init_hit_particles() {
    hitEnemy = Renderer::create_particle_system(5, 1000, V2(0, 0));
    hitEnemy.alive_time = {0.3, 0.6};
    hitEnemy.velocity_dir = {0, 2*PI};
    hitEnemy.spawn_size = {0.003, 0.005};
    hitEnemy.die_red = {0.960, 0.960};
    hitEnemy.die_green = {0.894, 0.894};
    hitEnemy.die_blue = {0.529, 0.529};
    hitEnemy.velocity = {0.1, 1};
}
