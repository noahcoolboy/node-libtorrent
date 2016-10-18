{
  'targets': [
    {
      'target_name': 'cmakebuild',
      'target_type': 'none',
      'actions': [
        {
          'action_name': 'run',
          'inputs': ['./CMakeLists.txt'],
          'outputs': ['./build'],
          'action': [ 'npm', 'run', 'compilation' ]
        }
      ]
    }
  ]
}
