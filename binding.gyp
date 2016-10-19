{
  'targets': [
    {
      'target_name': 'cmakebuild',
      'target_type': 'none',
      'actions': [
        {
          'variables': {
            'run_var': 'run',
            'cpl_var': 'compilation'
          },
          'action_name': 'cmakejsaction',
          'inputs': ['CMakeLists.txt'],
          'outputs': ['build'],
          'action': [ 'npm', '<(run_var)', '<(cpl_var)' ]
        }
      ]
    }
  ]
}
