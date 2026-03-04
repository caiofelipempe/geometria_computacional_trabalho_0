use bevy::prelude::*;
use bevy::render::mesh::{Indices, PrimitiveTopology};
use bevy::render::render_asset::RenderAssetUsages;
use parry3d::na::Point3;
use parry3d::transformation;
use rand::Rng;

fn main() {
    App::new()
        .add_plugins(DefaultPlugins)
        .add_systems(Startup, setup)
        .add_systems(Update, rotate_camera)
        .run();
}

#[derive(Component)]
struct MainCamera;

fn setup(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<StandardMaterial>>,
) {
    // Gerar pontos aleatórios
    let points = generate_random_points(100);

    // Calcular o convex hull, que retorna vértices e índices
    let (hull_vertices, hull_indices) = transformation::convex_hull(&points);

    // Criar mesh do convex hull
    let mesh = create_mesh_from_hull_data(hull_vertices, hull_indices);

    // Adicionar câmera
    commands.spawn((
        Camera3dBundle {
            transform: Transform::from_xyz(8.0, 8.0, 8.0).looking_at(Vec3::ZERO, Vec3::Y),
            ..default()
        },
        MainCamera,
    ));

    // Adicionar luz ambiente
    commands.insert_resource(AmbientLight {
        color: Color::WHITE,
        brightness: 0.3,
    });

    // Adicionar luz direcional
    commands.spawn(DirectionalLightBundle {
        directional_light: DirectionalLight {
            illuminance: 10000.0,
            shadows_enabled: true,
            ..default()
        },
        transform: Transform::from_xyz(4.0, 8.0, 4.0).looking_at(Vec3::ZERO, Vec3::Y),
        ..default()
    });

    // Adicionar o convex hull como entidade
    commands.spawn(PbrBundle {
        mesh: meshes.add(mesh),
        material: materials.add(StandardMaterial {
            base_color: Color::BLUE.with_a(0.8),
            alpha_mode: AlphaMode::Blend,
            perceptual_roughness: 0.3,
            metallic: 0.1,
            ..default()
        }),
        ..default()
    });

    // Adicionar pontos originais para referência
    spawn_points_visualization(&mut commands, &mut meshes, &mut materials, &points);

    // Adicionar eixos de coordenadas (opcional)
    spawn_axes(&mut commands, &mut meshes, &mut materials);
}

fn generate_random_points(num_points: usize) -> Vec<Point3<f32>> {
    let mut rng = rand::thread_rng();
    let mut points = Vec::with_capacity(num_points);

    for _ in 0..num_points {
        points.push(Point3::new(
            rng.gen_range(-2.0..2.0),
            rng.gen_range(-2.0..2.0),
            rng.gen_range(-2.0..2.0),
        ));
    }

    points
}

fn create_mesh_from_hull_data(
    vertices: Vec<Point3<f32>>,
    indices: Vec<[u32; 3]>,
) -> Mesh {
    let vertices_bevy: Vec<[f32; 3]> = vertices.into_iter().map(|p| p.coords.into()).collect();
    let indices_bevy: Vec<u32> = indices.into_iter().flat_map(|i| i.to_vec()).collect();

    let mut mesh = Mesh::new(
        PrimitiveTopology::TriangleList,
        RenderAssetUsages::RENDER_WORLD,
    );
    mesh.insert_attribute(Mesh::ATTRIBUTE_POSITION, vertices_bevy);
    mesh.insert_indices(Indices::U32(indices_bevy));
    mesh.duplicate_vertices();
    mesh.compute_flat_normals();
    mesh
}

fn spawn_points_visualization(
    commands: &mut Commands,
    meshes: &mut ResMut<Assets<Mesh>>,
    materials: &mut ResMut<Assets<StandardMaterial>>,
    points: &[Point3<f32>],
) {
    let sphere_mesh = meshes.add(Sphere { radius: 0.05 });
    let point_material = materials.add(Color::RED);

    for point in points {
        commands.spawn(PbrBundle {
            mesh: sphere_mesh.clone(),
            material: point_material.clone(),
            transform: Transform::from_xyz(point.x, point.y, point.z),
            ..default()
        });
    }
}

fn spawn_axes(
    commands: &mut Commands,
    meshes: &mut ResMut<Assets<Mesh>>,
    materials: &mut ResMut<Assets<StandardMaterial>>,
) {
    let axis_length = 4.0;
    let axis_radius = 0.02;

    // Eixo X (vermelho)
    commands.spawn(PbrBundle {
        mesh: meshes.add(Cylinder {
            radius: axis_radius,
            half_height: axis_length / 2.0,
        }),
        material: materials.add(Color::RED),
        transform: Transform::from_xyz(axis_length / 2.0, 0.0, 0.0)
            .with_rotation(Quat::from_rotation_z(std::f32::consts::FRAC_PI_2)),
        ..default()
    });

    // Eixo Y (verde)
    commands.spawn(PbrBundle {
        mesh: meshes.add(Cylinder {
            radius: axis_radius,
            half_height: axis_length / 2.0,
        }),
        material: materials.add(Color::GREEN),
        transform: Transform::from_xyz(0.0, axis_length / 2.0, 0.0),
        ..default()
    });

    // Eixo Z (azul)
    commands.spawn(PbrBundle {
        mesh: meshes.add(Cylinder {
            radius: axis_radius,
            half_height: axis_length / 2.0,
        }),
        material: materials.add(Color::BLUE),
        transform: Transform::from_xyz(0.0, 0.0, axis_length / 2.0)
            .with_rotation(Quat::from_rotation_x(std::f32::consts::FRAC_PI_2)),
        ..default()
    });
}

fn rotate_camera(
    time: Res<Time>,
    mut camera_query: Query<&mut Transform, With<MainCamera>>,
    mut angle: Local<f32>,
) {
    let radius = 10.0;
    *angle += time.delta_seconds() * 0.2;

    if *angle > std::f32::consts::TAU {
        *angle -= std::f32::consts::TAU;
    }

    for mut transform in camera_query.iter_mut() {
        transform.translation.x = radius * angle.sin();
        transform.translation.z = radius * angle.cos();
        transform.look_at(Vec3::ZERO, Vec3::Y);
    }
}
